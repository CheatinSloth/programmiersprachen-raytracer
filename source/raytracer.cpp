#include <renderer.hpp>
#include <window.hpp>

#include <GLFW/glfw3.h>
#include <thread>
#include <utility>
#include <cmath>
#include <fstream>
#include "scene.hpp"
#include "shape.hpp"

using namespace::std;

void parse(string fileName, Scene sdfScene) {


	vector<string> instructions;

	fstream file; // SDF File
	string line; // Individual Lines
	string token; // Individual strings of each line

	file.open(fileName, ios::in);
	if (file.is_open()) {
		cout << "File has been opened" << endl;

		// Filling vector over stringstream with each instruction of line
		while (getline(file, line)) {
			stringstream lineStream(line);
			instructions.clear();

			while (lineStream >> token) {
				instructions.push_back(token);
			}

			// Check to see if object should be initialized
			if (instructions[0] == "define") {
				cout << "Attempting to create " << instructions[1];


				// Block for creating Shape::Sphere and adding it to sceneShapes map
				if (instructions[1] == "shape" && instructions[2] == "sphere") {
					if (instructions.size() != 9) {
						cout << "Incorrect instruction syntax";
						break;
					}

					Sphere sdfSphere({ stof(instructions[4]), stof(instructions[5]), stof(instructions[6]) },
						stof(instructions[7]),
						sdfScene.sceneMaterial.at(instructions[8]),
						instructions[3]);
				}

				// Block for creating Shape::Box and adding it to sceneShapes map
				else if (instructions[1] == "shape" && instructions[2] == "box") {
					if (instructions.size() != 11) {
						cout << "Incorrect instruction syntax";
						break;
					}

					Box sdfBox({ stof(instructions[4]), stof(instructions[5]), stof(instructions[6]) }, // min
						{ stof(instructions[7]), stof(instructions[8]), stof(instructions[9]) }, // max
						sdfScene.sceneMaterial.at(instructions[10]),									 // material
						instructions[3]);														 // name

					sdfScene.sceneElements.emplace(instructions[3], sdfBox);
				}


				// Block for creating Material and adding it to sceneMaterials map
				else if (instructions[1] == "material") {
					if (instructions.size() != 13) {
						cout << "Incorrect instruction syntax";
						break;
					}
					Material sdfMaterial;
					sdfMaterial.name = instructions[2];
					sdfMaterial.ka = { stof(instructions[3]), stof(instructions[4]), stof(instructions[5]) };
					sdfMaterial.kd = { stof(instructions[6]), stof(instructions[7]), stof(instructions[8]) };
					sdfMaterial.ks = { stof(instructions[9]), stof(instructions[10]), stof(instructions[11]) };
					sdfMaterial.reflectionExponent = stof(instructions[12]);

					sdfScene.sceneMaterial.emplace(instructions[2], sdfMaterial);
				}

				// Block for creating Camera and adding it to sceneCameras map
				else if (instructions[1] == "camera") {
					if (instructions.size() != 9) {
						cout << "Incorrect instruction syntax";
						break;
					}
					Camera sdfCamera;
					sdfCamera.angle_ = stof(instructions[3]);
					sdfCamera.position_ = { stof(instructions[4]), stof(instructions[5]), stof(instructions[6]) };
					sdfCamera.resolutionV_ = { stoi(instructions[7]) };
					sdfCamera.resolutionH_ = { stoi(instructions[8]) };
					sdfScene.sceneCameras.insert(pair<string, Camera>(instructions[2], sdfCamera));
				}

				// Check for comment to simply skip line
				else if (instructions[1] == "#") {
					continue;
				}

				else {
					cout << "Incorrect instruction " << instructions[1] << " in File";
					break;
				}
			}

			// Missing define terminates program
			else {
				cout << "Incorrect instruction in File";
				break;
			}

		}
	}
	file.close();
}

//now single threaded again
int main(int argc, char* argv[])
{
  unsigned const image_width = 800;
  unsigned const image_height = 600;
  std::string const filename = "./checkerboard.ppm";

  Renderer renderer{image_width, image_height, filename};

  renderer.render();

  Window window{{image_width, image_height}};

  while (!window.should_close()) {
    if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      window.close();
    }
    window.show(renderer.color_buffer());
  }

  return 0;
}
