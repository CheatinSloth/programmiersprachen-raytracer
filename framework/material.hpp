#pragma once
#include <string>
#include "color.hpp"
#include "memory"

struct Material {

	friend std::ostream& operator<<(std::ostream& os, std::shared_ptr<Material> const mat) {
		std::cout << "Material: " << mat->name << std::endl << " ka: " << mat->ka
			<< std::endl << " kd: " << mat->kd
			<< std::endl << " ks: " << mat->ks
			<< std::endl << " Reflection Exponent: " << mat->reflectionExponent;
		return os;
	}
	std::string name{ "green" };
	Color ka{ 0.0f, 1.0f, 0.0f };
	Color kd{ 0.0f, 1.0f, 0.0f };
	Color ks{ 0.0f, 1.0f, 0.0f };
	float reflectionExponent = 100;
	float reflectivity = 0.f;
};
