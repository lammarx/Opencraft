#pragma once
#include <stdlib.h>
class Mesh
{
public:
	unsigned int vao;
	unsigned int vbo;
	size_t vertices;
	size_t vertexSize;

	Mesh(const float* buffer, size_t vertices, const int* attrs);
	~Mesh();
	
	void reload(const float* buffer, size_t vertices);
	void draw(unsigned int primitive);
};

