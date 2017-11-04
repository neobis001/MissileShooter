#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad\glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Camera.h>

#include "Shader.h"

enum DESTROY_CHECK {
	GREATER_THAN,
	LESS_THAN
};

enum AXIS {
	X,
	Y,
	Z
};


//
//a general missile class that's focused on mainly the data
//about the missile and manipulating it (like moving the missile)
class Missile {
private:
	glm::vec3 current_location;
	glm::vec3 forward_direction;
	float speed;

public:
	Missile();

	Missile(glm::vec3 starting_location, glm::vec3 forward_direction, float speed);

	//given a change in time deltaTime, update the current location of the missile instance
	//based on a forward movement given by forward_direction 
	void move(float deltaTime);

	//return current_location vector
	glm::vec3 get_current_loc();

};


//
//a missile class specifically for my Missile Shooter project
class MSMissile: public Missile {
private:
	bool should_be_destroyed = false;
	float destroy_value;
	AXIS destroy_axis;
	DESTROY_CHECK destroy_comparison;

	//
	//get a single component from missile's current location
	float get_current_loc_component(AXIS axis);

	//
	//with member variables about destroying the missile, check
	//the missile's current location, and update the missile status
	//to reflect whether it should be destroyed or not
	void check_destroy_status();

public:
	MSMissile();

	//
	//initialize instance and do an initial check to see if, given the parameters for destroying
	//a missile, whether the missile should be destroyed already or not
	MSMissile(glm::vec3 starting_location, glm::vec3 forward_direction, float speed, 
		float destroy_value, AXIS destroy_axis, DESTROY_CHECK destroy_comparison);

	void gl_draw_self(Shader& missileShader, Camera& camera, const glm::vec3& lamp_position,
		const glm::mat4& view, const glm::mat4& projection, GLint missileVAO, int element_count);

	void setup_msle_independent_shader_data(Shader& missileShader, Camera& camera_position, const glm::vec3& lamp_position,
		const glm::mat4& view, const glm::mat4& projection);

	void setup_msle_dependent_shader_data(Shader& missileShader);

	//
	//return whether the missile should be destroyed or not 
	bool get_destroy_status();

	//
	//move missile like normal Missile instance, and afterwards update status
	//to check whether missile should be destroyed or not
	void move(float deltaTime);

};