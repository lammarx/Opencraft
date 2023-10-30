#pragma once
#include <stdlib.h>
class Mesh
{
public:
	unsigned int vao;
	unsigned int vbo;
	size_t vertices;

	Mesh(const float* buffer, size_t vertices, const int* attrs);
	~Mesh();
	
	void draw(unsigned int primitive);
};

