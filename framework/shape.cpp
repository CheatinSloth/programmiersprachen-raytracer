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
mat_{} {}

Shape::Shape(string const& name, std::shared_ptr<Material> const& mats) :
name_{ name },
mat_ { mats }{}

std::ostream& Shape::print(std::ostream& os) const
{
    cout << "Name: " << name_ << " Material: (" << mat_->ka << "," << mat_->kd<< "," << mat_->ks << ","<< mat_->reflectionExponent<<")" << endl;
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


Sphere::Sphere(vec3 const& center, float radius, std::shared_ptr<Material> const& mats) :
Shape("outis", mats),
center_{center},
radius_{ radius } {}

Sphere::Sphere(vec3 const& center, float radius, std::shared_ptr<Material> const& mats, string const& name) :
Shape(name, mats),
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
        return HitPoint{true, t, name_, mat_, normalizedDirection*t, normalizedDirection};
    else
        return HitPoint{ false, t, name_, mat_, normalizedDirection, normalizedDirection };
}

Box::Box() :
Shape(),
min_ { 5.0f, 5.0f, 5.0f },
max_ { 20.0f, 20.0f, 20.0f } {}

Box::Box(vec3 const& min, vec3 const& max) :
Shape(),
min_{ min },
max_ { max }{}

Box::Box(vec3 const& min, vec3 const& max, std::shared_ptr<Material> const& mats) :

Shape("outis", mats),
min_{ min },
max_{ max } {}
	

Box::Box(vec3 const& min, vec3 const& max, std::shared_ptr<Material> const& mats, string const& name) :
Shape(name, mats),
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

HitPoint Box::intersect(Ray const& r, float& t) {
    float shortest_dis = INFINITY;
    float distances[6];

    float x_min = min_.x;
    t = (x_min - r.origin.x) / r.direction.x;
    glm::vec3 point_x_min = r.origin + t * r.direction;
    glm::vec3 vec = point_x_min - r.origin;
    distances[0] = sqrt(pow(vec.x,2)+pow(vec.y,2)+pow(vec.z,2));

    float x_max = max_.x;
    t = (x_max - r.origin.x) / r.direction.x;
    glm::vec3 point_x_max = r.origin + t * r.direction;
    vec = point_x_max - r.origin;
    distances[1] = sqrt(pow(vec.x,2)+pow(vec.y,2)+pow(vec.z,2));

    float y_min = min_.y;
    t = (y_min - r.origin.y) / r.direction.y;
    glm::vec3 point_y_min = r.origin + t * r.direction;
    vec = point_y_min - r.origin;
    distances[2] = sqrt(pow(vec.x,2)+pow(vec.y,2)+pow(vec.z,2));

    float y_max = max_.y;
    t = (y_max - r.origin.y) / r.direction.y;
    glm::vec3 point_y_max = r.origin + t * r.direction;
    vec = point_y_max - r.origin;
    distances[3] = sqrt(pow(vec.x,2)+pow(vec.y,2)+pow(vec.z,2));

    float z_min = min_.z;
    t = (z_min - r.origin.z) / r.direction.z;
    glm::vec3 point_z_min = r.origin + t * r.direction;
    vec = point_z_min - r.origin;
    distances[4] = sqrt(pow(vec.x,2)+pow(vec.y,2)+pow(vec.z,2));

    float z_max =max_.z;
    t = (z_max - r.origin.z) / r.direction.z;
    glm::vec3 point_z_max = r.origin + t * r.direction;
    vec = point_z_max - r.origin;
    distances[5] = sqrt(pow(vec.x,2)+pow(vec.y,2)+pow(vec.z,2));

    for (int i = 0; i<=5; i++){
        if(distances[i]<shortest_dis){
            if(distances[i]!=0) {
                shortest_dis = distances[i];
            }
        }
    }
    t = shortest_dis;
    glm::vec3 point = r.origin + t * r.direction;

    if (point.y >= min_.y && point.y <= max_.y) {
        if (point.z >= min_.z && point.z <= max_.z) {
            return HitPoint{true, t, name_, mat_, point, glm::normalize(r.direction)};
        } else {
            return HitPoint{false, t, name_, mat_, point, glm::normalize(r.direction)};
        }
    } else {
        return HitPoint{false, t, name_, mat_, point, glm::normalize(r.direction)};
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