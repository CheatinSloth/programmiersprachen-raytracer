#pragma once
#define _USE_MATH_DEFINES
#include <string>
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/gtx/intersect.hpp>
#include <iostream>
#include "color.hpp"
#include "ray.hpp"
#include "hitpoint.hpp"

using std::string;
using glm::vec3;

class Shape {
	
public:

	Shape();
	Shape(string const& name, Color const& color);
	virtual float area() = 0;
	virtual float volume() = 0;
	virtual std::ostream& print(std::ostream& os) const;
	
protected: 
	string name_;
	Color color_;
};

class Sphere : public Shape {
public:
	Sphere();
	Sphere(vec3 const& center);
	Sphere(vec3 const& center, float radius);
	Sphere(vec3 const& center, float radius, Color const& color);
	Sphere(vec3 const& center, float radius, Color const& color, string const& name);

	float area() const;
	float volume() const;
	std::ostream& print(std::ostream& os) const override;
	HitPoint intersect(Ray const& r);

protected: 
	vec3 center_;
	float radius_;
};

class Box : public Shape {
public:
	Box();
	Box(vec3 const& min, vec3 const& max);
	Box(vec3 const& min, vec3 const& max, Color const& color);
	Box(vec3 const& min, vec3 const& max, Color const& color, string const& name);

	float area() const;
	float volume() const;
	std::ostream& print(std::ostream& os) const override;


protected:
	vec3 min_;
	vec3 max_;
};

std::ostream& operator<<(std::ostream& os, Shape const& s);