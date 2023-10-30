#pragma once

#include <stdlib.h>

class Mesh;
class Chunk;

class VoxelRenderer
{
public:
	float* buffer;
	size_t capacity;
	VoxelRenderer(size_t capacity);
	~VoxelRenderer();

	Mesh* render(Chunk* chunk);
};

