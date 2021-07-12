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
    
    //E: x = min_ + q* v1 + s* v2
    //x1 = min_.x + q * v1.x + s * v2.x
    //x2 = min_.y + q * v1.y + s * v2.y
    //x3 = min_.z + q * v1.z + s * v2.z

    math;

    //konvertiere parameter in koordinaten form der ebene

    //setze Gerade in ebene ein
    //bestimmr r
    //setze r in Gerade ein um Schnittpunkt(e) zu errechnen
    //check welcher schnittpunkt die geringste distanz zum ray origin hat
    //return diesen hitpoint
    vec3 normalizedDirection = glm::normalize(r.direction);
    float p_x = min_.x;
    t = (min_.x - r.origin.x)/r.direction.x;
    float p_y = r.origin.y + t *r.direction.y;
    float p_z = r.origin.z + t *r.direction.z;

    vec3 dist_temp = {p_x,p_y,p_z};

    if (dist_temp.y >= min_.y && dist_temp.y <= max_.y)
    {
        if (dist_temp.z >= min_.z && dist_temp.z <= max_.z)
        {
            return HitPoint{true, t, name_, color_, normalizedDirection * t, normalizedDirection};
        }
        else
        {
            return HitPoint{false, t, name_, color_, normalizedDirection, normalizedDirection};
        }
    }
    else
    {
        return HitPoint{false, t, name_, color_, normalizedDirection, normalizedDirection};
    }
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
