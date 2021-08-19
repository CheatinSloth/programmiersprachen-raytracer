#pragma once
#include <glm/vec3.hpp>

struct Camera {
	float angle { 40.0f };
	glm::vec3 position { 1.0f, 1.0f, 1.0f };
	glm::vec3 direction{ 1.0f, 1.0f, 1.0f };
	glm::vec3 up{ 1.0f, 1.0f, 1.0f };
	int resolutionV { 800 };
	int resolutionH { 600 };
};