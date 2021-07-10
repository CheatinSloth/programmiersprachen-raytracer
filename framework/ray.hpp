#pragma once
#include <glm/vec3.hpp>
using glm::vec3;

struct Ray {
	vec3 origin = {0.0f, 0.0f, 0.0f};
	vec3 direction = { 0.0f, 0.0f, -1.0f };
};