#include "include\callbacks.h"

using namespace std;

float lastX = STARTING_LAST_MOUSE_X;
float lastY = STARTING_LAST_MOUSE_Y;

bool firstMouseMovement = true;

Camera* ref_camera;
bool ref_camera_initialized = false;


void glfw_error_callback(int error_code, const char * description) {
	cerr << "\n--GLFW ERROR CALLBACK--" << endl;
	cerr << "Error code is " << error_code << endl;
	cerr << "Description is " << description << endl;
	cerr << "--END--\n" << endl;
}


void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	float xPosFloat = (float)xPos;
	float yPosFloat = (float)yPos;

	if (firstMouseMovement) {
		lastX = xPosFloat;
		lastY = yPosFloat;
		firstMouseMovement = false;
	}

	float xOffset = xPosFloat - lastX;
	float yOffset = lastY - yPosFloat;
	lastX = xPosFloat;
	lastY = yPosFloat;

	if (!ref_camera_initialized) {
		CERR_ERROR(REF_CAMERA_UNINITIALIZED_ERROR);
		abort();
	}
	else {
		(*ref_camera).ProcessMouseMovement(xOffset, yOffset, true);
	}
}


void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	if (!ref_camera_initialized) {
		CERR_ERROR(REF_CAMERA_UNINITIALIZED_ERROR);
	}
	else {
		(*ref_camera).ProcessMouseScroll((float)yOffset);
	}
}

void initialize_ref_camera(Camera* camera_address) {
	ref_camera = camera_address;
	ref_camera_initialized = true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}