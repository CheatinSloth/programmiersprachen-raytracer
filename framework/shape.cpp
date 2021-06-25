#include "shape.hpp"
using std::string;
using glm::vec3;

Sphere::Sphere() :
	center_{0.0f, 0.0f, 0.0f},
	radius_{ 1.0f }, 
	color_{1.0f, 1.0f, 1.0f} {}

Sphere::Sphere(vec3 center) :
	center_{ center },
	radius_{1.0f} {}


Sphere::Sphere(vec3 center, float radius) :
	center_{ center },
	radius_{radius} {}


Sphere::Sphere(vec3 center, float radius, Color color) :
	center_{center},
	radius_{ radius } {}

Sphere::Sphere(vec3 center, float radius, Color color, string name) :
	center_{ center },
	radius_{ radius },
	color_{ color },
	name_{ name } {}

// A = 4*pi*r
float Sphere::area()
{
	return 4 * M_PI * radius_;
}
// V = 4/3*pi*r^3
float Sphere::volume()
{
	return (4.0f/3.0f) * M_PI * std::pow(radius_, 3.0f);
}

Box::Box()
{
	min_ = vec3{ 0.0f, 0.0f, 0.0f };
	max_ = vec3{ 1.0f, 1.0f, 1.0f };
}

Box::Box(vec3 min, vec3 max)
{
	min_ = min;
	max_ = max;
}

Box::Box(vec3 min, vec3 max, Color color)
{
	min_ = min;
	max_ = max;
	color_ = color;
}

Box::Box(vec3 min, vec3 max, Color color, string name)
{
	min_ = min;
	max_ = max;
	color_ = color;
	name_ = name;
}

float Box::area()
{
	float h = sqrt(pow(max_.y - min_.y, 2.0f));
	float w = sqrt(pow(max_.x - min_.x, 2.0f));
	float l = sqrt(pow(max_.z - min_.z, 2.0f));

	return (2*h*l) + (2*h*w) + (2*w*l);
}

float Box::volume()
{
	float h = sqrt(pow(max_.y - min_.y, 2.0f));
	float w = sqrt(pow(max_.x - min_.x, 2.0f));
	float l = sqrt(pow(max_.z - min_.z, 2.0f));

	return h*w*l;
}
