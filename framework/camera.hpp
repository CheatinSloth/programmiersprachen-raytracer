#pragma once

#define _USE_MATH_DEFINES
#include <glm/glm.hpp>
#include <string>
#include <cmath>

struct Camera {
	std::string name_;
	float angle { 40.0f };
	glm::vec3 position { 0.0f, 0.0f, 0.0f };
	glm::vec3 direction { 0.0f, 0.0f, -1.0f };
	glm::vec3 up { 0.0f, 1.0f, 0.0f };
	int resolutionV { 800 };
	int resolutionH { 600 };

	float dist() const;
	glm::mat4 transform() const;
};
