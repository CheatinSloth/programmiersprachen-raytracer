#pragma once
#include <glm/vec3.hpp>
#include <string>

struct Camera {
	std::string name;
	float angle { 40.0f };
	glm::vec3 position { 1.0f, 1.0f, 1.0f };
	glm::vec3 direction{ 0.0f, 0.0f, -1.0f };
	glm::vec3 up{ 0.0f, 1.0f, 0.0f };
	int resolutionV { 800 };
	int resolutionH { 600 };
};