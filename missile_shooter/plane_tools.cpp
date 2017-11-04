#include "include\plane_tools.h"

float planeVerts[] = {
	PLANE_DIMENSION, 0.0f, PLANE_DIMENSION, 0.0f, 1.0f, 0.0f,
	-PLANE_DIMENSION, 0.0f, PLANE_DIMENSION, 0.0f, 1.0f, 0.0f,
	-PLANE_DIMENSION, 0.0f, -PLANE_DIMENSION, 0.0f, 1.0f, 0.0f,
	PLANE_DIMENSION, 0.0f, PLANE_DIMENSION, 0.0f, 1.0f, 0.0f,
	PLANE_DIMENSION, 0.0f, -PLANE_DIMENSION, 0.0f, 1.0f, 0.0f,
	-PLANE_DIMENSION, 0.0f, -PLANE_DIMENSION, 0.0f ,1.0f, 0.0f
};


void create_plane_VAO(GLuint& VBO_holder, GLuint& VAO_holder) {
	glGenBuffers(1, &VBO_holder);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_holder);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVerts), planeVerts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &VAO_holder);
	glBindVertexArray(VAO_holder);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_holder);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void gl_setup_planeshader(Shader& planeShader, const Camera& camera,
	const glm::vec3& lamp_position, const Mats& matrices) {

	//
	//setup the shader
	planeShader.Use();

	//
	//setup non-matrix uniforms
	GLint cLoc = glGetUniformLocation(planeShader.programId, "cameraPos");
	glUniform3f(cLoc, camera.Position.x, camera.Position.y, camera.Position.z);

	//
	//setup material struct
	GLint maLoc = glGetUniformLocation(planeShader.programId, "material.ambientColor");
	GLint mdLoc = glGetUniformLocation(planeShader.programId, "material.diffuseColor");
	GLint msLoc = glGetUniformLocation(planeShader.programId, "material.specularColor");
	GLint mshLoc = glGetUniformLocation(planeShader.programId, "material.shininess");

	glUniform3f(maLoc, 1.0f, 0.5f, 0.31f);
	glUniform3f(mdLoc, 1.0f, 0.5f, 0.31f);
	glUniform3f(msLoc, 0.5f, 0.5f, 0.5f);
	glUniform1f(mshLoc, 32.0f);

	//
	//setup light struct
	GLint laLoc = glGetUniformLocation(planeShader.programId, "light.ambientColor");
	GLint ldLoc = glGetUniformLocation(planeShader.programId, "light.diffuseColor");
	GLint lsLoc = glGetUniformLocation(planeShader.programId, "light.specularColor");
	GLint lpLoc = glGetUniformLocation(planeShader.programId, "light.position");

	glUniform3f(laLoc, 0.2f, 0.2f, 0.2f);
	glUniform3f(ldLoc, 0.5f, 0.5f, 0.5f);
	glUniform3f(lsLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lpLoc, lamp_position.x, lamp_position.y, lamp_position.z);

	//
	//set up view matrix
	GLint vLoc = glGetUniformLocation(planeShader.programId, "view");
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(matrices.view));

	//
	//set up projection matrix
	GLint pLoc = glGetUniformLocation(planeShader.programId, "projection");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(matrices.projection));

	//
	//set up model matrix
	GLint mLoc = glGetUniformLocation(planeShader.programId, "model");
	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(matrices.model));

	//
	//set up normal matrix
	GLint nLoc = glGetUniformLocation(planeShader.programId, "normal");
	glUniformMatrix3fv(nLoc, 1, GL_FALSE, glm::value_ptr(matrices.normal));

	glUseProgram(0);
}

void gl_draw_plane(Shader& planeShader, GLuint planeVAO) {
	planeShader.Use();
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(0);
}