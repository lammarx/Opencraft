#pragma once

#include <stdlib.h>
#include <glm/glm.hpp>

using namespace glm;

class Chunk;
class voxel;

class ChunkEngine
{
public:
	Chunk** chunks;
	size_t volume;
	unsigned int w, h, d;

	ChunkEngine(int w, int h, int d);
	~ChunkEngine();

	Chunk* getChunk(int x, int y, int z);
	voxel* get(int x, int y, int z);
	void set(int x, int y, int z, int id);
	voxel* raycast(vec3 start, vec3 dir, float maxLength, vec3& end, vec3& norm, vec3& iend);
};

