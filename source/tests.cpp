#define CATCH_CONFIG_RUNNER
#define _USE_MATH_DEFINES
#include <catch.hpp>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <iostream>
#include <string>
#include "color.hpp"
#include "shape.hpp"
#include "ray.hpp"
#include "hitpoint.hpp"


TEST_CASE("intersect_ray_sphere", "[intersect]")
{
	// Ray
	glm::vec3 ray_origin{ 0.0f, 0.0f, 0.0f };
	// ray direction has to be normalized !
	// you can use :
	// v = glm :: normalize ( some_vector )
	glm::vec3 ray_direction{ 0.0f, 0.0f, 1.0f };
	// Sphere
	glm::vec3 sphere_center{ 0.0f, 0.0f, 5.0f };
	float sphere_radius{ 1.0f };
	float distance = 0.0f;
	auto result = glm::intersectRaySphere(
		ray_origin, ray_direction,
		sphere_center,
		sphere_radius * sphere_radius, // squared radius !!!
		distance);
	REQUIRE(distance == Approx(4.0f));
}

TEST_CASE("intersect_ray_box", "[intersect]")
{
    //ray construction
    float t = 0.0f;
    glm::vec3 rai = {0.0f, 0.0f, 0.0f};
    glm::vec3 rai_dir = {0.0f, 1.0f, 0.0f};
    Ray brrr;
    brrr.origin = rai;
    brrr.direction = rai_dir;
    Box Peter{{-1.0f,1.0f,0.0f},{1.0f, 2.0f, 1.0f}};
    HitPoint boi = Peter.intersect(brrr, t);
    REQUIRE(boi.hit == true);
    REQUIRE(boi.dist == Approx(3.0f));
    REQUIRE(boi.touchPoint.x == Approx(0.0f));
    REQUIRE(boi.touchPoint.y == Approx(0.0f));
    REQUIRE(boi.touchPoint.z == Approx(3.0f));
}


int main(int argc, char *argv[])
{
  return Catch::Session().run(argc, argv);
}
