#pragma once
#define _USE_MATH_DEFINES
#include <string>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <iostream>
#include "color.hpp"
#include "ray.hpp"
#include "hitpoint.hpp"
#include "material.hpp"

using std::string;
using glm::vec3;
using glm::mat4;

class Shape {
	
public:

    Shape();
    Shape(string const& name, std::shared_ptr<Material> const& mats);
    virtual float const area() = 0;
    virtual float const volume() = 0;
    virtual std::ostream& print(std::ostream& os) const;
    virtual HitPoint const intersect(Ray const& ray) = 0;
    void set_world_mat(mat4 worldMat);
    void set_world_inv();
	
protected: 
    string name_;
    std::shared_ptr<Material> mat_;
    mat4 world_transformation_{ 1.0f, 0.f, 0.f, 0.f,
                                0.0f, 1.f, 0.f, 0.f,
                                0.0f, 0.f, 1.f, 0.f,
                                0.0f, 0.f, 0.f, 1.f 
    };
    mat4 world_transformation_inv_{glm::inverse(world_transformation_)};
};

class Sphere : public Shape {
public:
    Sphere();
    Sphere(vec3 const& center);
    Sphere(vec3 const& center, float radius);
    Sphere(vec3 const& center, float radius, std::shared_ptr<Material> const& mats);
    Sphere(vec3 const& center, float radius, std::shared_ptr<Material> const& mats, string const& name);

    float const area() override;
    float const volume() override;
    std::ostream& print(std::ostream& os) const override;
    HitPoint const intersect(Ray const& r) override;

protected: 
    vec3 center_;
    float radius_;
};

class Box : public Shape {
public:
    Box();
    Box(vec3 const& min, vec3 const& max);
    Box(vec3 const& min, vec3 const& max, std::shared_ptr<Material> const& mats);
    Box(vec3 const& min, vec3 const& max, std::shared_ptr<Material> const& mats, string const& name);

    float const area() override;
    float const volume() override;
    bool const inBox(glm::vec3);
    std::ostream& print(std::ostream& os) const override;
    HitPoint const intersect(Ray const& ray) override;

protected:
    // smallest coordinate tuple | in -z: (bottom right corner furthest from viewer)
    vec3 min_;
    // largest coordinate tuple | in -z: (top left corner closest to viewer)
    vec3 max_;
};

Ray transformRay(glm::mat4 const& mat, Ray const& ray);
std::ostream& operator<<(std::ostream& os, Shape const& s);