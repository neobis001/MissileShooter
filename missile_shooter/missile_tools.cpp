#include "include\missile_tools.h"

bool process_click = true;


void create_missile_VAO(GLuint& missileVBO, GLuint& missileVAO, int& num_elements) {
	std::vector<float> missile_data;
	
	load_model_data("media\\model_data.txt", missile_data, num_elements);
	
	int missile_data_size = missile_data.size();
	float* missile_verts = new float[missile_data_size];
	for (int i = 0; i < missile_data_size; i++) {
		*(missile_verts + i) = missile_data[i];
	}

	glGenBuffers(1, &missileVBO);
	glBindBuffer(GL_ARRAY_BUFFER, missileVBO);
	glBufferData(GL_ARRAY_BUFFER, missile_data_size * sizeof(float), missile_verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] missile_verts; //buffer data apparently copied, so can safely de-allocate data now

	glGenVertexArrays(1, &missileVAO);

	glBindVertexArray(missileVAO);

	glBindBuffer(GL_ARRAY_BUFFER, missileVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool process_missile_click(GLFWwindow* window) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS &&
		process_click) {
		process_click = false;
		return true;
	}
	else { //at this point, the GLFW_PRESS could still be true (just process_click check failed)
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
			process_click = true;
		}

		return false;
	}
}

void process_missiles_to_destroy(std::vector<MSMissile>& missiles) {
	if (missiles.size() == 0) {
		return;
	}

	std::vector<MSMissile>::iterator checkIter = missiles.begin();
	while (checkIter != missiles.end()) {
		MSMissile missile_to_check = *checkIter;
		if (missile_to_check.get_destroy_status() == true) {
			checkIter = missiles.erase(checkIter);
		}
		else {
			checkIter++; //only increment if erase not done (since in erase, iterator may become
			//actual end)
		}
	}
}

void move_missiles(std::vector<MSMissile>& missiles, float deltaTime) {
	std::vector<MSMissile>::iterator mIter;

	for (mIter = missiles.begin(); mIter != missiles.end(); mIter++) {
		MSMissile& missile_to_move = *mIter;
		missile_to_move.move(deltaTime);
	}
}