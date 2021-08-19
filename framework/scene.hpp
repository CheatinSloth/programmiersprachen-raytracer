#pragma once
#include <vector>
#include <map>
#include "ray.hpp"
#include "camera.hpp"
#include "shape.hpp"
using std::vector;
using std::map;

struct Scene {
	map<string, Shape> sceneElements;
	vector<Ray> lightSources;
	vector<Ray> baseLighting;
	map<string, Camera> sceneCameras;
	map<string, std::shared_ptr<Material> > sceneMaterial;
};