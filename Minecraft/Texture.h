#pragma once

#include <string>
#include <GL/glew.h>

class Texture
{
public:
	int width;
	int height;
	int nrChannels;
	unsigned char* data;
	GLuint texture;

	Texture(const char* path);
	~Texture();

	void bind();
};
