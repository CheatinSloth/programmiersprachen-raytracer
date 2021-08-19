#pragma once
#include <string>
#include "color.hpp"
struct Material {
	std::string name{ "green" };
	Color ka{ 0.0f, 1.0f, 0.0f };
	Color kd{ 0.0f, 1.0f, 0.0f };
	Color ks{ 0.0f, 1.0f, 0.0f };
	float reflectionExponent = 50;
};
std::ostream& operator<<(std::ostream& os, std::shared_ptr<Material> const& mat);