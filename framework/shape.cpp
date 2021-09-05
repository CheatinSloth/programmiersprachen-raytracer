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

void Shape::set_world_mat(mat4 worldMat)
{
    world_transformation_ = worldMat;
}

void Shape::set_world_inv()
{
    world_transformation_inv_ = glm::inverse(world_transformation_);
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

HitPoint const Sphere::intersect(Ray const& r)
{
    float t;
    vec3 hitpoint, hitnormal;
    Ray rayTrans{transformRay(world_transformation_inv_, r)};
    vec3 normalizedDirection = glm::normalize(rayTrans.direction);

    if (glm::intersectRaySphere(rayTrans.origin, normalizedDirection, center_, radius_, hitpoint, hitnormal)) {
   
        vec3 distanceVec = hitpoint - rayTrans.origin;
        t = sqrt(pow(distanceVec.x, 2) + pow(distanceVec.y, 2) + pow(distanceVec.z, 2));

        vec3 normalVec{(hitpoint - center_)};

        mat4 transp = glm::transpose(world_transformation_inv_);
        vec3 normalTrans(transp * glm::vec4{ normalVec.x, normalVec.y, normalVec.z, 0.f });
        normalVec = glm::normalize(normalTrans);

        return HitPoint{ true, t, name_, mat_, hitpoint, normalizedDirection, normalVec };
    }
    else
        return HitPoint{};
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

bool const Box::inBox(glm::vec3 point){

    float offset = 0.0001f;

    if (point.x + offset >= min_.x && point.x - offset <= max_.x && point.y + offset >= min_.y && point.y - offset <= max_.y && point.z + offset >= min_.z && point.z - offset <= max_.z) {
        return true;
    }
    return false;
}

HitPoint const Box::intersect(Ray const& r) {

    glm::vec3 raynorm = glm::normalize(r.direction);
    bool hit = false;
    Ray rayTrans{transformRay(world_transformation_inv_, r)};
    float shortest_dis = INFINITY;
    float t;
    vec3 sideNorm{0.f, 0.f, 0.f};     // Creates normal vector for orthogonal boxes


    // left
    float x_min = min_.x;
    t = (x_min - r.origin.x) / raynorm.x;
    glm::vec3 point_x_min = r.origin + t * raynorm;
    glm::vec3 vec = point_x_min - r.origin;
    if ( inBox(point_x_min) && glm::length(vec) < shortest_dis ){
        shortest_dis = glm::length(vec);
        sideNorm = {-1.f, 0.f, 0.f};
    }

    // right
    float x_max = max_.x;
    t = (x_max - r.origin.x) / raynorm.x;
    glm::vec3 point_x_max = r.origin + t * raynorm;
    vec = point_x_max - r.origin;
    if ( inBox(point_x_max) && glm::length(vec) < shortest_dis ){
        shortest_dis = glm::length(vec);
        sideNorm = {1.f, 0.f, 0.f};
    }

    // bottom
    float y_min = min_.y;
    t = (y_min - r.origin.y) / raynorm.y;
    glm::vec3 point_y_min = r.origin + t * raynorm;
    vec = point_y_min - r.origin;
    if ( inBox(point_y_min) && glm::length(vec) < shortest_dis ){
        shortest_dis = glm::length(vec);
        sideNorm = {0.f, -1.f, 0.f};
    }

    // top
    float y_max = max_.y;
    t = (y_max - r.origin.y) / raynorm.y;
    glm::vec3 point_y_max = r.origin + t * raynorm;
    vec = point_y_max - r.origin;
    if ( inBox(point_y_max) && glm::length(vec) < shortest_dis ){
        shortest_dis = glm::length(vec);
        sideNorm = {0.f, 1.f, 0.f};
    }

    // back (in -z)
    float z_min = min_.z;
    t = (z_min - r.origin.z) / raynorm.z;
    glm::vec3 point_z_min = r.origin + t * raynorm;
    vec = point_z_min - r.origin;
    if(inBox(point_z_min) && glm::length(vec)<shortest_dis){
        shortest_dis = glm::length(vec);
        sideNorm = {0.f, 0.f, -1.f};
    }

    // front (in -z)
    float z_max = max_.z;
    t = (z_max - r.origin.z) / raynorm.z;
    glm::vec3 point_z_max = r.origin + t * raynorm;
    vec = point_z_max - r.origin;
    if ( inBox(point_z_max) && glm::length(vec) < shortest_dis ){
        shortest_dis = glm::length(vec);
        sideNorm = {0.f, 0.f, 1.f};
    }

    vec3 point = (r.origin + shortest_dis * raynorm) + 0.0001f * sideNorm;
    vec3 normalTrans{r.origin + t * rayTrans.direction};


    return HitPoint{shortest_dis!=INFINITY, shortest_dis, name_, mat_, point, raynorm, sideNorm};
}

Ray transformRay(glm::mat4 const& mat, Ray const& ray)
{
    glm::vec4 origin{ ray.origin, 1.f };
    glm::vec4 direction{ ray.direction, 0.f };

    vec3 transOrigin{ origin * mat };
    vec3 transDirection{ direction * mat };

    return Ray{ transOrigin, transDirection };
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