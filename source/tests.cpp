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


TEST_CASE("Shape::Sphere", "[Sphere]")
{
	Sphere bob = { {4.0f, 4.0f, 4.0f}, 10.0f, {1.0f, 1.0f, 1.0f}, "Bob" };
	Sphere tom = { {4.0f, 4.0f, 4.0f}, 7.34f, {0.33f, 0.33f, 0.33f}, "Tom" };
	Sphere phil = { {4.0f, 4.0f, 4.0f}, -5.0f, {0.0f, 0.0f, 0.0f}, "Phil" };

	SECTION("area") {
		REQUIRE(bob.area() == Approx(1256.64f));
		REQUIRE(tom.area() == Approx(677.02f));
		REQUIRE(phil.area() == Approx(314.16f));
	}
	SECTION("volume") {
		REQUIRE(bob.volume() == Approx(4188.79f));
		REQUIRE(tom.volume() == Approx(1656.44f));
		REQUIRE(phil.volume() == Approx(523.6f));
	}
}

TEST_CASE("Shape::Box", "[Box]")
{
	Box bob = { {4.0f, 4.0f, 4.0f}, {6.0f, 6.0f, 6.0f}, {1.0f, 1.0f, 1.0f}, "Bob" };
	Box tom = { {0.0f, 0.0f, 0.0f}, {10.0f, 11.0f, 10.0f}, {0.33f, 0.33f, 0.33f}, "Tom" };
	Box phil = { {-2.0f, -2.0f, 3.0f}, {12.0f, 5.0f, 13.0f}, {0.0f, 0.0f, 0.0f}, "Phil" };
	
	SECTION("area") {
		REQUIRE(bob.area() == Approx(0.0f));
		REQUIRE(tom.area() == Approx(0.0f));
		REQUIRE(phil.area() == Approx(0.0f));
	}
	SECTION("volume") {
		REQUIRE(bob.volume() == Approx(0.0f));
		REQUIRE(tom.volume() == Approx(0.0f));
		REQUIRE(phil.volume() == Approx(0.0f));
	}
	std::cout << bob << std::endl;
	std::cout << tom << std::endl;
	std::cout << phil << std::endl;
	
}



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
    glm::vec3 rai_dir = {0.0f, 0.0f, 1.0f};
    Ray brrr;
    brrr.origin = rai;
    brrr.direction = rai_dir;
    Box Peter{{-2.0f,0.0f,3.0f},{2.0f, 1.0f, 4.0f}};
    HitPoint boi = Peter.intersect(brrr, t);
    //REQUIRE(boi.hit == true);
    REQUIRE(boi.dist == Approx(1.0f));
    REQUIRE(boi.touchPoint.x == Approx(0.0f));
    REQUIRE(boi.touchPoint.y == Approx(0.0f));
    REQUIRE(boi.touchPoint.z == Approx(3.0f));
}


int main(int argc, char *argv[])
{
  return Catch::Session().run(argc, argv);
}
