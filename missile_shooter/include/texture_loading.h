#pragma once
#include <glad\glad.h>
#include <stb_image.h>


#define STBI_TRUE 1
#define STBI_FALSE 0

void load_texture(const char * filename, bool flip_vertical, int internalformat,
	int format, GLuint& tex, bool& success);