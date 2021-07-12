#pragma once
#include <glm/vec3.hpp>

struct Camera {
	float angle_{ 40.0f };
	glm::vec3 position_{ 1.0f, 1.0f, 1.0f };
	int resolutionV_{ 500 };
	int resolutionH_{ 500 };
};