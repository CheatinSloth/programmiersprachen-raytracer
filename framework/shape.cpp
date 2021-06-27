#include "shape.hpp"
using std::string;
using glm::vec3;
using std::cout;
using std::cin;
using std::endl;

Shape::Shape() :
	name_{ "outis" },
	color_{ 1.0f, 1.0f, 1.0f } {}

Shape::Shape(string const& name, Color const& color) :
	name_{ name },
	color_{ color } {}

std::ostream& Shape::print(std::ostream& os) const
{
	cout << "Name: " << name_ << " Color: (" << color_.r << "," << color_.g << "," << color_.b << ")" << endl;
	return os;
}

Sphere::Sphere() :
	Shape(),
	center_{ 10.0f, 10.0f, 10.0f },
	radius_{ 1.0f } {}

Sphere::Sphere(vec3 const& center) :
	Shape(),
	center_{ center },
	radius_{1.0f} {}


Sphere::Sphere(vec3 const& center, float radius) :
	Shape(),
	center_{ center },
	radius_{radius} {}


Sphere::Sphere(vec3 const& center, float radius, Color const& color) :
	Shape("outis", color),
	center_{center},
	radius_{ radius } {}

Sphere::Sphere(vec3 const& center, float radius, Color const& color, string const& name) :
	Shape(name, color),
	center_{ center },
	radius_{ radius }{}

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

std::ostream& Sphere::print(std::ostream& os) const
{
	Shape::print(os) << " Center: (" << center_.x << "," << center_.y << "," << center_.z << ") Radius: " << radius_ << endl;
	return os;
}

Box::Box() :
	Shape(),
	min_ { 5.0f, 5.0f, 5.0f },
	max_ { 20.0f, 20.0f, 20.0f } {}


Box::Box(vec3 const& min, vec3 const& max) :
	Shape(),
	min_{ min },
	max_ {max}{}

Box::Box(vec3 const& min, vec3 const& max, Color const& color) :

	Shape("outis", color),
	min_{ min },
	max_{ max } {}
	

Box::Box(vec3 const& min, vec3 const& max, Color const& color, string const& name) :
	Shape(name, color),
	min_{ min },
	max_{ max } {}

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

std::ostream& Box::print(std::ostream& os) const
{
	Shape::print(os) << "Min: (" << min_.x << "," << min_.y << "," << min_.z << ") Max: (" << max_.x << "," << max_.y << "," << max_.z << ")" << endl;
	return os;
}

std::ostream& operator<<(std::ostream& os, Shape const& s)
{
	s.print(os);
	return os;

}
