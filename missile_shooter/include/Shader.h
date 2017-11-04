#pragma once

#include <glad\glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "error_macro.h"

#define INFOLOG_SIZE 512

class Shader {
private:
	bool compile_success = false; //set to true on
	  //constructor finish

	bool compile_shaders(const char* vshader_source, const char* fshader_source);

public:
	GLuint programId;

	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	// use/activate the shader
	void Use();

	void Delete();

	bool isSuccessCompiled();

	/*
	// utility uniform functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	*/

};