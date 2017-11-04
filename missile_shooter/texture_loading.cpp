#include "include\texture_loading.h"

void load_texture(const char * filename, bool flip_vertical, int internalformat,
	int format, GLuint& tex, bool& success) {
	//
	//load container.jpg texture

	int width, height, nrChannels;

	if (flip_vertical) {
		stbi_set_flip_vertically_on_load(STBI_TRUE);
	}

	unsigned char * img_data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (img_data == NULL) {
		success = false;
		return;
	}

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	/*
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //if zooming out on an object
	//no need to do extra calculations via GL_LINEAR to show smooth color, not worth it there
	//so GL_NEAREST is better option here
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, img_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(img_data);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (flip_vertical) {
		stbi_set_flip_vertically_on_load(STBI_FALSE); //reset if needed
	}

	success = true;
}