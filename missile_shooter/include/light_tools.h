#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"

//
//for the white cube light, create a VAO based on harcoded data in
//light_tools.cpp, and return references to VBO and VAO used in process
void create_light_VAO(GLuint& lightVBO, GLuint& lightVAO);

//
//given a shader object and data objects representing a lamp,
//draw the lamp
void gl_draw_lamp(Shader& lampShader, GLuint lightVAO, const glm::vec3& lamp_position,
	const glm::mat4& view, const glm::mat4& projection);