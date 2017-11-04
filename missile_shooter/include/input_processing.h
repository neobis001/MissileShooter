#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "Camera.h"

//
//process ESC key for closing
//process WASD for moving camera position to be later used in a glm::lookat command for a view matrix
void process_input(GLFWwindow* window, Camera& camera, float deltaTime);