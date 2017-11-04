#include "include\missile_loading.h"

void load_model_data(const char * filename, std::vector<float>& res_data, int& num_elements) {
	std::vector<float> data_to_return;

	ifstream ifile(filename);

	if (ifile.fail()) {
		CERR_ERROR("file loading failed");
		abort();
	}

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float nx = 0.0f;
	float ny = 0.0f;
	float nz = 0.0f;

	while (ifile >> x >> y >> z >> nx >> ny >> nz) { //while file not done reading
		  //with every >> operation on a cin stream, returns cin object
		  //cin object can be evaluated here for fail/EOF checks

		data_to_return.push_back(x);
		data_to_return.push_back(y);
		data_to_return.push_back(z);
		data_to_return.push_back(nx);
		data_to_return.push_back(ny);
		data_to_return.push_back(nz);
	}


	if (!is_valid_model_data(data_to_return)) {
		char error_msg[200];
		sprintf_s(error_msg, 200, "Invalid model data. Number of Component data numbers found"
			" was %i, which is not a multiple of %i, the number of data points per triangle",
			data_to_return.size(), DATA_POINTS_PER_TRI);
		
		CERR_ERROR(error_msg);
		ifile.close();

		glfwTerminate();
		abort();
	}

	res_data = data_to_return; //for consistency, assign a complete-data vector to res_data instead of updating res_data during the process
	num_elements = res_data.size() / NUM_DATA_POINTS_PER_ELEMENT; //assuming is valid model data, use division to calculate number of elements
		//expected to be rendered in a VAO
}


bool is_valid_model_data(const std::vector<float>& data) {
	return (data.size() % DATA_POINTS_PER_TRI == 0); //one-liner check can remove need for
	//if-else statements, got feedback about this, but still, more natural/easier for me to just
	//write out if else statements first, add one-liners on feedback or future revisions/refining
}