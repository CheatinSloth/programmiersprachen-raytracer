#include "shape.hpp"
#include "cmath"
#include "iostream"
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
float const Sphere::area()
{
	return 4 * M_PI * abs(radius_);
}

// V = 4/3*pi*r^3
float const Sphere::volume()
{
	return (4.0f/3.0f) * M_PI * std::pow(abs(radius_), 3.0f);
}

std::ostream& Sphere::print(std::ostream& os) const
{
	Shape::print(os) << "Center: (" << center_.x << "," << center_.y << "," << center_.z << ") Radius: " << radius_ << endl;
	return os;
}

HitPoint Sphere::intersect(Ray const& r, float& t)
{
	vec3 normalizedDirection = glm::normalize(r.direction);
	if(glm::intersectRaySphere(r.origin, normalizedDirection, center_, radius_ * radius_, t))
		return HitPoint{true, t, name_, color_, normalizedDirection*t, normalizedDirection};
	else
		return HitPoint{ false, t, name_, color_, normalizedDirection, normalizedDirection };
}

Box::Box() :
	Shape(),
	min_ { 5.0f, 5.0f, 5.0f },
	max_ { 20.0f, 20.0f, 20.0f } {}

Box::Box(vec3 const& min, vec3 const& max) :
	Shape(),
	min_{ min },
	max_ { max }{}

Box::Box(vec3 const& min, vec3 const& max, Color const& color) :

	Shape("outis", color),
	min_{ min },
	max_{ max } {}
	

Box::Box(vec3 const& min, vec3 const& max, Color const& color, string const& name) :
	Shape(name, color),
	min_{ min },
	max_{ max } {}

float const Box::area()
{
	float h = sqrt(pow(max_.y - min_.y, 2.0f));
	float w = sqrt(pow(max_.x - min_.x, 2.0f));
	float l = sqrt(pow(max_.z - min_.z, 2.0f));

	return (2*h*l) + (2*h*w) + (2*w*l);
}

float const Box::volume()
{
	float h = sqrt(pow(max_.y - min_.y, 2.0f));
	float w = sqrt(pow(max_.x - min_.x, 2.0f));
	float l = sqrt(pow(max_.z - min_.z, 2.0f));

	return h*w*l;
}

HitPoint Box::intersect(Ray const& r, float& t)
{
    /*
    float q,s,closes_dis;
    //Ray als geraden funktion
     glm ::vec3 boi = r.origin + t * r.direction;
    //calculate plane
    //using min_ and max_ to construct planes
    //min_, p1{min_.x, min_.y, max_.z}, p2{min_.x, max_.y, min_.z}
    glm::vec3 p1 = {min_.x, min_.y, max_.z};
    glm::vec3 p2 = {min_.x, max_.y, min_.z};
    //Richtungsvektoren für die Ebene
    glm::vec3 v1 = p1 -min_;
    glm::vec3 v2 = p2 -min_;

    glm::vec3 norm = {(v1.y*v2.z)-(v1.z*v2.y), (v1.z*v2.x)-(v1.x*v2.z), (v1.x*v2.y)-(v1.y*v2.x)};
    float a = p1.x*norm.x;
    float b = p1.y*norm.y;
    float c = p1.z*norm.z;
    float d = a + b + c;
    //setze Gerade in ebene ein
    //bestimmr r
    q = (-a*r.origin.x-b*r.origin.y-c*r.origin.z+d)/(a*r.direction.x+b*r.direction.y+c*r.direction.z);
    glm::vec3 point;
    //setze r in Gerade ein um Schnittpunkt(e) zu errechnen
    float fin_x = r.origin.x + q * r.direction.x;
    float fin_y = r.origin.y + q * r.direction.y;
    float fin_z = r.origin.z + q * r.direction.z;

    point = {fin_x,fin_y,fin_z};

    if (point.y >= min_.y && point.y <= max_.y)
    {
        if (point.z >= min_.z && point.z <= max_.z)
        {
            return HitPoint{true, t, name_, color_, point * t, r.direction};
        }
        else
        {
            return HitPoint{false, t, name_, color_, point, r.direction};
        }
    }
    else
    {
        return HitPoint{false, t, name_, color_, point, r.direction};
    }
    //check welcher schnittpunkt die geringste distanz zum ray origin hat
    //return diesen hitpoint
   */
    



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
