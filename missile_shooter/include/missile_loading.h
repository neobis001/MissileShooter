#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <GLFW/glfw3.h>

#include "include\universal_macros.h"

#define NUM_POSITION_COMPONENTS 3
#define NUM_NORMAL_COMPONENTS 3
#define DATA_POINTS_PER_TRI ((NUM_POSITION_COMPONENTS + NUM_NORMAL_COMPONENTS) * 3)

#define NUM_DATA_POINTS_PER_ELEMENT (NUM_POSITION_COMPONENTS + NUM_NORMAL_COMPONENTS)

using namespace std;

void load_model_data(const char * filename, std::vector<float>& res_data, int& num_elements);

bool is_valid_model_data(const std::vector<float>& data);