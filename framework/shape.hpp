#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <glm/vec3.hpp>

class Shape {
public: 
	virtual float area() = 0;
	virtual float volume() = 0;
};

class Sphere : public Shape {
public:
	Sphere();
	Sphere(glm::vec3 center);
	Sphere(glm::vec3 center, float radius);
	float area();
	float volume();
protected: 
	glm::vec3 center_;
	float radius_;
};

class Box : public Shape {
public:
	Box();
	Box(glm::vec3 min, glm::vec3 max);
	float area();
	float volume();

protected:
	glm::vec3 min_;
	glm::vec3 max_;
};