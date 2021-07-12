#pragma once
#include <vector>
#include "ray.hpp"
#include "camera.hpp"
#include "shape.hpp"
using std::vector;

struct Scene {
	vector<Shape> elements;
	vector<Ray> lightSources;
	vector<Ray> baseLighting;
	Camera camera;
};