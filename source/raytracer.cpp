#include <renderer.hpp>
#include <window.hpp>
#include <GLFW/glfw3.h>
#include <thread>
#include <utility>
#include <cmath>
#include <fstream>  // Necessary for file reading
#include <sstream>  // Necessary for tokenizing lines
#include "scene.hpp"
#include "shape.hpp"

using namespace::std;

// Testing for access token 
//Parser 
void parse(string const& fileName, Scene& sdfScene, Renderer& renderer) {
	cout << "Parsing started" << endl;
	vector<string> instructions;

	fstream file; // SDF File
	string line; // Individual Lines
	string token; // Individual strings of each line

	// Open file 
	file.open(fileName);
	if (file.is_open()) {
		cout << "File has been opened" << endl;

		// Fills a vector with individual strings of each word in line 
		while (getline(file, line)) {
			stringstream lineStream(line);

			// In case vector is full from last reading, vector is cleared
			instructions.clear();

			// Push individual word (token) into vector 
			while (lineStream >> token) {
				instructions.push_back(token);
			}

			// Check to see if object should be initialized
			if (instructions[0] == "define") {
				cout << "Attempting to create " << instructions[1] << endl;

				// Block for creating Shape::Sphere and adding it to sceneShapes map
				if (instructions[1] == "shape" && instructions[2] == "sphere") {
					if (instructions.size() != 9) {
						cout << "Incorrect instruction syntax. Sphere::Shape requires 9 tokens." << endl;
						break;
					}

					sdfScene.sceneElements.emplace(instructions[3],												// Places sphere at name
						new Sphere({ stof(instructions[4]), stof(instructions[5]), stof(instructions[6]) },     // Center
							stof(instructions[7]),																// Radius
							sdfScene.sceneMaterial.at(instructions[8]),											// Material
							instructions[3]));																	// Name
				}

				// Block for creating Shape::Box and adding it to sceneShapes map
				else if (instructions[1] == "shape" && instructions[2] == "box") {
					if (instructions.size() != 11) {
						cout << "Incorrect instruction syntax. Box::Shape requires 11 tokens." << endl;
						break;
					}

					sdfScene.sceneElements.emplace(instructions[3],													// Places Box at name
						new Box({ stof(instructions[4]), stof(instructions[5]), stof(instructions[6]) },			// min
							{ stof(instructions[7]), stof(instructions[8]), stof(instructions[9]) },			// max
							sdfScene.sceneMaterial.at(instructions[10]),										// Material
							instructions[3]));																	// name
				}


				// Block for creating Material and adding it to sceneMaterials map
				else if (instructions[1] == "material") {
					if (instructions.size() != 13) {
						cout << "Incorrect instruction syntax. Material requires 13 tokens." << endl;
						break;
					}
					Material sdfMaterial{
						instructions[2],																			// name
						{ stof(instructions[3]), stof(instructions[4]), stof(instructions[5]) },					// ka
						{ stof(instructions[6]), stof(instructions[7]), stof(instructions[8]) },					// kd
						{ stof(instructions[9]), stof(instructions[10]), stof(instructions[11]) },					// ks
						stof(instructions[12]) };																	// reflectionExponent

					sdfScene.sceneMaterial.emplace(instructions[2], make_shared<Material>(sdfMaterial));

				}
				// Block for creating Camera and adding it to sceneCameras map
				else if (instructions[1] == "camera") {
					if (instructions.size() == 13) {
						Camera sdfCamera{
							instructions[2],																			// name
							stof(instructions[3]),																		// angle
							{ stof(instructions[4]), stof(instructions[5]), stof(instructions[6]) },					// position
							{ stof(instructions[7]), stof(instructions[8]), stof(instructions[9]) },					// direction
							{ stof(instructions[10]), stof(instructions[11]), stof(instructions[12]) },					// up
							800,																						// resolutionV
							600 
						};																						// resolutionH

						sdfScene.sceneCameras.emplace(instructions[2], sdfCamera);
					}

					if (instructions.size() == 4) {
						Camera sdfCamera{ instructions[2], stof(instructions[3]) };
						sdfScene.sceneCameras.emplace(instructions[2], sdfCamera);
					}
					else {
						cout << "Incorrect instruction syntax. Camera requires 4 or 13 tokens." << endl;
					}

				}

				// Block for creating light source and adding it to lightsource map
				else if (instructions[1] == "light") {
					if (instructions.size() != 10) {
						cout << "Incorrect instruction syntax. Light requires 10 tokens." << endl;
						break;
					}

					light sdfLight{
						instructions[2],																			// name								
						{ stof(instructions[3]), stof(instructions[4]), stof(instructions[5]) },					// position
						{ stof(instructions[6]) ,stof(instructions[7]) ,stof(instructions[8]) },					// hue
						stoi(instructions[9])																		// luminance
					};																	
					sdfScene.lightSources.emplace(instructions[2], sdfLight);
				}			

				// No matching term will end loop
				else {
					cout << "Incorrect instruction " << instructions[1] << " in File";
					break;
				}
			}

			// Set ambient lighting
			else if (instructions[0] == "ambient") {
				if (instructions.size() != 4) {
					cout << "Incorrect instruction syntax. Shape requires 4 tokens." << endl;
					break;
				}
				sdfScene.baseLighting = { stof(instructions[1]), stof(instructions[2]), stof(instructions[3]) };
			}

			// Transforms
			else if (instructions[0] == "transform") {
			cout << "Attempting to transform " << instructions[1] << endl;
			if (instructions[1] == "scale") {
				if (instructions.size() != 5) {
					cout << "Incorrect instruction syntax. Transformation scale requires 5 tokens." << endl;
				}

			}
			if (instructions[1] == "translate") {
				if (instructions.size() != 5) {
					cout << "Incorrect instruction syntax. Transformation translate requires 5 tokens." << endl;
				}
			}
			if (instructions[1] == "rotate") {
				if (instructions.size() != 6) {
					cout << "Incorrect instruction syntax. Transformation translate requires 6 tokens." << endl;
				}
			}
			}
		
			else if (instructions[0] == "render") {
			cout << "Starting render" << endl;
			//renderer{ stoul(instructions[3]), stoul(instructions[4]), instructions[2] };
			renderer.render(sdfScene, sdfScene.sceneCameras.at(instructions[1]));
			}

			// Check for comment to simply skip line
			else if (instructions[0] == "#") {
			continue;
			}

			// Missing "define" terminates program
			else {
				cout << "Incorrect instruction in File: '" << instructions[0] << "' is not a valid token" << endl;
				break;
			}
		}
			
	}
	cout << "Parsing complete" << endl;
	file.close();
}

//now single threaded again
int main(int argc, char* argv[])
{

  unsigned const image_width = 800;
  unsigned const image_height = 600;
  string const filename = "./checkerboard.ppm";


  Renderer renderer{image_width, image_height, filename};

  Scene scene;
  scene.baseLighting = { 0.f, 1.f, 0.f };

  parse("test.sdf", scene, renderer);


  cout << scene.sceneMaterial.size() << " Materials in scene" << endl;
  cout << scene.sceneElements.size() << " Elements in scene" << endl;
  cout << scene.lightSources.size() << " Lights in scene" << endl;

  Window window{{image_width, image_height}};

  while (!window.should_close()) {
    if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      window.close();
    }
    window.show(renderer.color_buffer());
  }

  return 0;
}
