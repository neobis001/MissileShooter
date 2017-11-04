/*
OpenGL Missile Shooter
by Nicolas Obias, completed 10/27/17
(Physical requirements spec. in own possession)

This project aims on having the user roam around a scene containing an orange plane
and a white light cube (symbolizing the light for lighting calculations) in a free moving/rotating
camera, with the ability to left click to make a missile appear on one side of the plane and disappear on the other.

The program loads vertex model data from media\model_data.txt. The .txt file consists of triangle data
listed as 3 lines of vertices, where each line is a single vertex. The format of a single vertex line is:
{x} {y} {z} {normal x} {normal y} {normal z}
All 6 of these inputs for a single vertex should be a floating point number with 3 decimal digits.
Although I specified in my own requirements spec. a specific model shape, this model data setup is flexible to
load any model, as long as it's valid data.

WASD to move
Move mouse to rotate camera
Scroll up/down to zoom in/out on scene.
Left click down to make a missile appear.

*/

#include <glad\glad.h>

#include <GLFW\glfw3.h>

#include <iostream>
#include <cmath>
#include <vector>
#include <Camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include\universal_macros.h"

#include "include\Shader.h"
#include "include\texture_loading.h" //do I really need this module?
#include "include\callbacks.h" 
#include "include\input_processing.h"
#include "include\plane_tools.h"
#include "include\light_tools.h"
#include "include\missile_tools.h"

using namespace std;

//
//hardcoded camera properties
#define INITIAL_YAW -45.0f
#define STARTING_Y 1.0f

//
//hardcoded values to determine missile properties
#define MISSILE_HALF_LENGTH 0.5f //gotten from looking at model

const float missile_start_value = PLANE_DIMENSION - MISSILE_HALF_LENGTH;
const glm::vec3 missile_start_loc(0.0f, 2.0f, missile_start_value);
const glm::vec3 missile_forward_dir(0.0f, 0.0f, -1.0f);
const float missile_speed = 20.0f;
const float destroy_value = -missile_start_value;
AXIS destroy_axis = Z;
DESTROY_CHECK destroy_comparison = LESS_THAN;

//
//for calculating times
float deltaTime = 0.0f;
float lastFrameTime = 0.0f; 
float currentFrameTime = 0.0f;

//
//hardcoded camera object
Camera camera(glm::vec3(-PLANE_DIMENSION, STARTING_Y, PLANE_DIMENSION), glm::vec3(0.0f, 1.0f, 0.0f),
	INITIAL_YAW, 0.0f);

int main() {
	//
	//do own initialization stuff
	initialize_ref_camera(&camera);
	//
	//initialize glfw settings, create a window, and make opengl context current w/
	//created window
	
	glfwSetErrorCallback(glfw_error_callback);

	int window_init_res = glfwInit();
	if (window_init_res == GLFW_FALSE) {
		CERR_ERROR("glfwInit failed");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(START_W, START_H, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		CERR_ERROR("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	int screenWidth, screenHeight;
	glfwGetWindowSize(window, &screenWidth, &screenHeight);
	float aspectRatio = ((float)screenWidth) / ((float)screenHeight);

	//
	//Load OpenGL functions w/ GLAD
	//and initialize viewport and sizing settings
	//note that even though context already made, w/ GLAD usage
	//must load functions via GLAD even before using 1.1 functions

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		CERR_ERROR("Failed to initialize GLAD and function loading");
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, START_W, START_H);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//
	//setup Shader objects
	

	Shader planeShader("vshaders\\plane.vshader", "fshaders\\plane.fshader");
	if (!planeShader.isSuccessCompiled()) {
		CERR_ERROR("planeShader compilation failed");
		glfwTerminate();
		return -1;
	}


	Shader lampShader("vshaders\\lamp.vshader", "fshaders\\lamp.fshader");
	if (!lampShader.isSuccessCompiled()) {
		CERR_ERROR("lampShader compilation failed");
		glfwTerminate();
		return -1;
	}

	Shader missileShader("vshaders\\missile.vshader", "fshaders\\missile.fshader");
	if (!missileShader.isSuccessCompiled()) {
		CERR_ERROR("missileShader compilation fialed");
		glfwTerminate();
		return -1;
	}

	//setup planeVAO
	GLuint planeVBO;
	GLuint planeVAO;
	create_plane_VAO(planeVBO, planeVAO);

	//
	//setup light VAO
	GLuint lightVBO;
	GLuint lightVAO;
	create_light_VAO(lightVBO, lightVAO);

	//
	//setup missile VAO
	GLuint missileVBO;
	GLuint missileVAO;
	int num_missile_elems;
	create_missile_VAO(missileVBO, missileVAO, num_missile_elems);

	//
	//initialize buffer settings and local variables for loop
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //some dark green color
	glEnable(GL_DEPTH_TEST);

	glm::vec3 plane_position(0.0f, 0.0f, 0.0f);
	glm::vec3 lamp_position(0.0f, 5.0f, 0.0f);

	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 model;
	glm::mat3 normal;

	GLint mLoc = 0;
	GLint vLoc = 0;
	GLint pLoc = 0;

	std::vector<MSMissile> missiles;
	std::vector<MSMissile>::iterator mIter;

	while (!glfwWindowShouldClose(window)) {
		currentFrameTime = (float)glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
		
		process_missiles_to_destroy(missiles);
		move_missiles(missiles, deltaTime);

		process_input(window, camera, deltaTime);
		bool should_add_missile = process_missile_click(window);
		if (should_add_missile) {
			MSMissile new_missile(missile_start_loc, missile_forward_dir, missile_speed, destroy_value,
				destroy_axis, destroy_comparison);
			missiles.push_back(new_missile);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//
		//set up view and projection matrix numbers
		view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
		projection = glm::perspective(glm::radians(camera.Zoom), aspectRatio, 0.1f, 100.0f);

		//--
		//
		//setup and draw plane
		model = glm::mat4(1.0f);
		model = glm::translate(model, plane_position);

		normal = glm::mat3(glm::transpose(glm::inverse(model)));

		Mats plane_matrices;
		plane_matrices.model = model;
		plane_matrices.view = view;
		plane_matrices.projection = projection;
		plane_matrices.normal = normal;
		gl_setup_planeshader(planeShader, camera, lamp_position, plane_matrices);
		
		//
		//draw plane, lamp, and missiles

		gl_draw_plane(planeShader, planeVAO);
		gl_draw_lamp(lampShader, lightVAO, lamp_position, view, projection);
		for (mIter = missiles.begin(); mIter != missiles.end(); mIter++) {
			MSMissile missile_to_render = *mIter;
			missile_to_render.gl_draw_self(missileShader, camera, lamp_position, view, projection,
				missileVAO, num_missile_elems);

		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &lightVBO);
	glDeleteBuffers(1, &missileVBO);

	planeShader.Delete();
	lampShader.Delete();
	missileShader.Delete();

	glfwTerminate();
	return 0;
}


