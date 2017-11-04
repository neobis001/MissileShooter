#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Camera.h>

#include "Shader.h"
#include "universal_macros.h"

//
//a struct type representing the matrices needed in the gl_setup_planeshader
//function
typedef struct Mats_Template {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat3 normal;
} Mats;

void create_plane_VAO(GLuint& VBO_holder, GLuint& VAO_holder);

void gl_setup_planeshader(Shader& planeShader, const Camera& camera,
	const glm::vec3& lamp_position, const Mats& matrices);

void gl_draw_plane(Shader& planeShader, GLuint planeVAO);