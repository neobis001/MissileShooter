#pragma once
#include <glad\glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Camera.h"
#include "error_macro.h"

#define START_W 800
#define START_H 600
#define STARTING_LAST_MOUSE_X START_W / 2
#define STARTING_LAST_MOUSE_Y START_H / 2
#define REF_CAMERA_UNINITIALIZED_ERROR "ref_camera pointer hasn't been initialized yet"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void process_input(GLFWwindow* window);

void glfw_error_callback(int error_code, const char * description);

//
//process mouse positions to rotate camera as needed
//I think should only worry about gimbal lock if doing any roll, but only pitch (w/ yOffset) and yaw
//(w/ xOffset) considered here
void mouse_callback(GLFWwindow* window, double xPos, double yPos); //seems similar to the process_input callback
																   //but that's more for key input, this captures mouse info.

//
//process scrolling for controlling zoom (fov in a glm::lookAt call)
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

void initialize_ref_camera(Camera* camera_address);