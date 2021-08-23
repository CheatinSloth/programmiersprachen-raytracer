#pragma once
#include <glm/vec3.hpp>
#include <string>
#include "color.hpp"
#include "material.hpp"

using glm::vec3;
using std::string;

struct HitPoint {
	bool hit{ false };
	float dist{ INFINITY };
	string shapeName = "null";
	std::shared_ptr<Material> mat;
	vec3 touchPoint { 0.0f, 0.0f, -1.0f };
	vec3 direction { 0.0f, 0.0f, -1.0f };
};