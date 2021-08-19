#pragma once
#include <vector>
#include <map>
#include "ray.hpp"
#include "camera.hpp"
#include "shape.hpp"
#include "light.hpp"

using std::vector;
using std::map;

struct Scene {
	
	map<string, Shape> sceneElements;
	map<string, light> lightSources;
	Color baseLighting{0.1f, 0.1f, 0.1f};
	map<string, Camera> sceneCameras;
	map<string, std::shared_ptr<Material> > sceneMaterial;
};