#pragma once

#include <glm/glm.hpp>

using namespace glm;

class Camera
{
public:
	vec3 front;
	vec3 up;
	vec3 right;
	mat4 rotation;

	float fov;
	vec3 position;

	Camera(vec3 position, float fov);

	void rotate(float x, float y, float z);

	mat4 getProjection();
	mat4 getView();

private:
	void updateVectors();
};

