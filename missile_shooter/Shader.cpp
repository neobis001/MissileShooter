#include "include\Shader.h"

bool Shader::compile_shaders(const char* vshader_source, const char* fshader_source) {
	bool compilation_result = false;

	//
	//setup vertex shader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vshader_source, NULL);
	glCompileShader(vertexShader);

	GLint success = 0;
	GLchar infoLog[INFOLOG_SIZE];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, INFOLOG_SIZE, NULL, infoLog);
		std::cerr << "Error: Vertex Shader Compilation failed. Info: " << infoLog << std::endl;
		return compilation_result;
	}

	//
	//setup fragment shader

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fshader_source, NULL);
	glCompileShader(fragmentShader);

	success = 0;
	memset(infoLog, 0, sizeof(GLchar) * INFOLOG_SIZE);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, INFOLOG_SIZE, NULL, infoLog);
		std::cerr << "Error: Fragment Shader Compilation failed. Info: " << infoLog << std::endl;
		return compilation_result;
	}

	//
	//set up program object

	programId = glCreateProgram();
	glAttachShader(programId, vertexShader);
	glAttachShader(programId, fragmentShader);
	glLinkProgram(programId);

	success = 0;
	memset(infoLog, 0, sizeof(GLchar) * INFOLOG_SIZE);
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, INFOLOG_SIZE, NULL, infoLog);
		std::cerr << "Error: Shader Program Linking failed. Info: " << infoLog << std::endl;
		return compilation_result;
	}

	glDeleteShader(vertexShader); //docs say shaders are flagged for deletion if attached to program
								  //object, and when program object is deleted, then shaders are deleted
	glDeleteShader(fragmentShader);
	
	compilation_result = true;
	return compilation_result;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream;
		std::stringstream fShaderStream;

		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		CERR_ERROR("Attempting to read vertex and shader source files failed");
		return;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	bool compilation_result = compile_shaders(vShaderCode, fShaderCode);
	compile_success = compilation_result;
}

void Shader::Use() {
	glUseProgram(programId);
}

void Shader::Delete() {
	glDeleteProgram(programId);
}

bool Shader::isSuccessCompiled() {
	return compile_success;
}

 