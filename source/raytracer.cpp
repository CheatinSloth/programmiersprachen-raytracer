#include <renderer.hpp>
#include <window.hpp>
#include <GLFW/glfw3.h>
#include <thread>
#include <utility>
#include <cmath>
#include <fstream>
#include "scene.hpp"
#include "shape.hpp"
#include "sstream"

using namespace::std;

// Testing for access token 
//Parser 
void parse(string const& fileName, Scene sdfScene) {
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

						sdfScene.sceneElements.emplace(instructions[3], new Sphere(
							{ stof(instructions[4]), stof(instructions[5]), stof(instructions[6]) }, // Center
							stof(instructions[7]),													 // Radius			 
							sdfScene.sceneMaterial.at(instructions[8]),                              // Material             
							instructions[3]));                                                       // Name
				}

				// Block for creating Shape::Box and adding it to sceneShapes map
				else if (instructions[1] == "shape" && instructions[2] == "box") {
					if (instructions.size() != 11) {
						cout << "Incorrect instruction syntax";
						break;
					}			            
					
					sdfScene.sceneElements.emplace(instructions[3], new Box(
						{ stof(instructions[4]), stof(instructions[5]), stof(instructions[6]) }, // Min
						{ stof(instructions[7]), stof(instructions[8]), stof(instructions[9]) }, // Max	         
						sdfScene.sceneMaterial.at(instructions[10]),							 // Material			      
						instructions[3]));                                                       // Name
				}


				// Block for creating Material and adding it to sceneMaterials map
				else if (instructions[1] == "material") {
					if (instructions.size() != 13) {
						cout << "Incorrect instruction syntax";
						break;
					}

					Material sdfMaterial  
					{ instructions[2],															 // Name
					{ stof(instructions[3]), stof(instructions[4]), stof(instructions[5]) },     // ka
					{ stof(instructions[6]), stof(instructions[7]), stof(instructions[8]) },     // kd 
					{ stof(instructions[9]), stof(instructions[10]), stof(instructions[11]) },   // ks
					stof(instructions[12]) };													 // reflection Exponent

					sdfScene.sceneMaterial.emplace(instructions[2], make_shared<Material>(sdfMaterial));

				}
				// Block for creating Camera and adding it to sceneCameras map
				else if (instructions[1] == "camera") {
					if (instructions.size() != 13) {
						cout << "Incorrect instruction syntax";
						break;
					}

					Camera sdfCamera
					{ instructions[2],															 // name
					stof(instructions[3]),														 // angle
					{ stof(instructions[4]), stof(instructions[5]), stof(instructions[6]) },     // position
					{ stof(instructions[7]), stof(instructions[8]), stof(instructions[9]) },     // direction
					{ stof(instructions[10]), stof(instructions[11]), stof(instructions[12])},   // up
					800, 600};																     // resolution
					
					sdfScene.sceneCameras.emplace(instructions[2], sdfCamera);
				}

				// Block for creating light source and adding it to lightsource map
				else if (instructions[1] == "light") {
					if (instructions.size() != 10) {
						cout << "Incorrect instruction syntax";
						break;
					}

					light sdfLight
					{ instructions[2],															// name
					{ stof(instructions[3]) ,stof(instructions[4]) ,stof(instructions[5]) },    // position
					{ stof(instructions[6]) , stof(instructions[7]) ,stof(instructions[8]) },   // hue
					stoi(instructions[9]) };													// luminance

					sdfScene.lightSources.emplace(instructions[2],  sdfLight);
				}

				else if (instructions[1] == "transform") {
					if (instructions[2] == "scale") {}
					if (instructions[2] == "translate") {}
					if (instructions[2] == "rotate") {}
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

			// Set ambient lighting
			else if (instructions[0] == "ambient") {
				if (instructions.size() != 4) {
					cout << "Incorrect instruction syntax";
					break;
				}
				sdfScene.baseLighting = { stof(instructions[1]), stof(instructions[2]), stof(instructions[3]) };
			}

			// Missing "define" terminates program
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
  string const filename = "./checkerboard.ppm";

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
