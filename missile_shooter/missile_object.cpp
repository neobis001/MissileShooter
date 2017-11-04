#include "include\missile_object.h"

Missile::Missile() {

}

Missile::Missile(glm::vec3 starting_location, glm::vec3 forward_direction, float speed) :
	current_location(starting_location), forward_direction(forward_direction), speed(speed) {

}

void Missile::move(float deltaTime) {
	current_location += (speed * deltaTime) * forward_direction;
}

glm::vec3 Missile::get_current_loc() {
	return current_location;
}

float MSMissile::get_current_loc_component(AXIS axis) {
	glm::vec3 current_loc = get_current_loc();

	float res_component = 0.0f;
	switch (axis) {
	case X: {
		res_component = current_loc.x;
		break;
	}
	case Y: {
		res_component = current_loc.y;
		break;
	}
	case Z: {
		res_component = current_loc.z;
		break;
	}
	default: {
		CERR_ERROR("Invalid axis input");
		abort();
		break;
	}
	}

	return res_component;
}

MSMissile::MSMissile() {

}


MSMissile::MSMissile(glm::vec3 starting_location, glm::vec3 forward_direction, float speed,
	float destroy_value, AXIS destroy_axis, DESTROY_CHECK destroy_comparison):
	Missile(starting_location, forward_direction, speed), destroy_value(destroy_value), destroy_axis(destroy_axis),
	destroy_comparison(destroy_comparison) {

}


void MSMissile::gl_draw_self(Shader& missileShader, Camera& camera, const glm::vec3& lamp_position,
	const glm::mat4& view, const glm::mat4& projection, GLint missileVAO, int element_count) {
	setup_msle_independent_shader_data(missileShader, camera, lamp_position, view, projection);
	setup_msle_dependent_shader_data(missileShader);

	missileShader.Use();
	glBindVertexArray(missileVAO);

	glDrawArrays(GL_TRIANGLES, 0, element_count);
	glBindVertexArray(0);
	glUseProgram(0);
}

void MSMissile::setup_msle_independent_shader_data(Shader& missileShader, Camera& camera, const glm::vec3& lamp_position,
	const glm::mat4& view, const glm::mat4& projection) {
	//--
	//
	//setup uniforms that are the same for all missiles

	missileShader.Use();
	GLint cLoc = glGetUniformLocation(missileShader.programId, "cameraPos");
	glUniform3f(cLoc, camera.Position.x, camera.Position.y, camera.Position.z);

	//
	//setup material struct
	GLint maLoc = glGetUniformLocation(missileShader.programId, "material.ambientColor");
	GLint mdLoc = glGetUniformLocation(missileShader.programId, "material.diffuseColor");
	GLint msLoc = glGetUniformLocation(missileShader.programId, "material.specularColor");
	GLint mshLoc = glGetUniformLocation(missileShader.programId, "material.shininess");

	glUniform3f(maLoc, 1.0f, 0.5f, 0.31f);
	glUniform3f(mdLoc, 0.074f, 0.666f, 0.192f);
	glUniform3f(msLoc, 0.5f, 0.5f, 0.5f);
	glUniform1f(mshLoc, 32.0f);

	//
	//setup light struct
	GLint laLoc = glGetUniformLocation(missileShader.programId, "light.ambientColor");
	GLint ldLoc = glGetUniformLocation(missileShader.programId, "light.diffuseColor");
	GLint lsLoc = glGetUniformLocation(missileShader.programId, "light.specularColor");
	GLint lpLoc = glGetUniformLocation(missileShader.programId, "light.position");

	glUniform3f(laLoc, 0.2f, 0.2f, 0.2f);
	glUniform3f(ldLoc, 0.5f, 0.5f, 0.5f);
	//glUniform3f(lsLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lsLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lpLoc, lamp_position.x, lamp_position.y, lamp_position.z);

	GLint vLoc = glGetUniformLocation(missileShader.programId, "view");
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLint pLoc = glGetUniformLocation(missileShader.programId, "projection");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glUseProgram(0);
}

void MSMissile::setup_msle_dependent_shader_data(Shader& missileShader) {
	missileShader.Use();

	glm::mat4 model(1.0f);
	model = glm::translate(model, get_current_loc()); //hardcoded these transformation matrices
	  //based on the model I made in blender with the requirements spec.
	  //to consider the matrix order, consider the last transformation listed here is like the
	  //first matrix to be applied (like a stack model)
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	GLint mLoc = glGetUniformLocation(missileShader.programId, "model");
	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat3 normal = glm::mat3(glm::transpose(glm::inverse(model)));
	GLint nLoc = glGetUniformLocation(missileShader.programId, "normal");
	glUniformMatrix3fv(nLoc, 1, GL_FALSE, glm::value_ptr(normal));

	glUseProgram(0);

}

void MSMissile::check_destroy_status() {
	float comparison_component = get_current_loc_component(destroy_axis);

	if (destroy_comparison == LESS_THAN) {
		if (comparison_component < destroy_value) {
			should_be_destroyed = true;
		}
		else {
			should_be_destroyed = false;
		}
	}
	else { //must be greater than
		if (comparison_component > destroy_value) {
			should_be_destroyed = true;
		}
		else {
			should_be_destroyed = false;
		}
	}
}

bool MSMissile::get_destroy_status() {
	return should_be_destroyed;
}

void MSMissile::move(float deltaTime) {
	Missile::move(deltaTime);
	check_destroy_status();
}