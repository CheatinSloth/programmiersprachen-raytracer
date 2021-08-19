#pragma once
#include "color.hpp"
#include <glm/vec3.hpp>

struct light {
	std::string name{ "light source" };
	glm::vec3 position{ 100.0f, 100.0f, 100.0f };
	Color hue{ 1.0f, 1.0f, 1.0f };
	int luminance{ 10 };
};