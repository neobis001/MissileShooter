#pragma once
#include <glad\glad.h>
#include <vector>

#include "missile_loading.h"
#include "missile_object.h"

//
//provided a hardcoded file location for the missile vertex data, create the VBO and corresponding VAO
//to represent the missile, and return them
void create_missile_VAO(GLuint& missileVBO, GLuint& missileVAO, int& num_elements);

bool process_missile_click(GLFWwindow* window);

//
//check (if there are any) missile objects in missiles list and remove
//any that are marked to be destroyed
void process_missiles_to_destroy(std::vector<MSMissile>& missiles);

//
//process movement for each missile in missiles list based on the
//same change in time deltaTime
void move_missiles(std::vector<MSMissile>& missiles, float deltaTime);