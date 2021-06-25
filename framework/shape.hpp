#pragma once
#define _USE_MATH_DEFINES
#include <string>
#include <cmath>
#include <glm/vec3.hpp>
#include "color.hpp"

using std::string;
using glm::vec3;

class Shape {
	
public: 
	virtual float area() = 0;
	virtual float volume() = 0;
	string name_;
	Color color_;
};

class Sphere : public Shape {
public:
	Sphere();
	Sphere(vec3 center);
	Sphere(vec3 center, float radius);
	Sphere(vec3 center, float radius, Color color);
	Sphere(vec3 center, float radius, Color color, string name);

	float area();
	float volume();

protected: 
	vec3 center_;
	float radius_;
};

class Box : public Shape {
public:
	Box();
	Box(vec3 min, vec3 max);
	Box(vec3 min, vec3 max, Color color);
	Box(vec3 min, vec3 max, Color color, string name);
	float area();
	float volume();

protected:
	vec3 min_;
	vec3 max_;
};