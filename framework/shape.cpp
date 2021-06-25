#include "shape.hpp"

Sphere::Sphere()
{
	center_ = glm::vec3{0.0f, 0.0f, 0.0f};
	radius_ = 1.0f;
}

Sphere::Sphere(glm::vec3 center)
{
	center_ = center;
	radius_ = 1.0f;
}

Sphere::Sphere(glm::vec3 center, float radius)
{
	center_ = center;
	radius_ = radius;
}

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
	min_ = glm::vec3{ 0.0f, 0.0f, 0.0f };
	max_ = glm::vec3{ 1.0f, 1.0f, 1.0f };
}

Box::Box(glm::vec3 min, glm::vec3 max)
{
	min_ = min;
	max_ = max;
}

float Box::area()
{
	return (0.0f);
}
