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
#include "iostream"

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

					sdfScene.sceneElements.emplace(instructions[3],												// Places Box at name
						new Box({ stof(instructions[4]), stof(instructions[5]), stof(instructions[6]) },		// min
							{ stof(instructions[7]), stof(instructions[8]), stof(instructions[9]) },			// max
							sdfScene.sceneMaterial.at(instructions[10]),										// Material
							instructions[3]));																	// name
				}


				// Block for creating Material and adding it to sceneMaterials map
				else if (instructions[1] == "material") {
					if (instructions.size() != 14) {
						cout << "Incorrect instruction syntax. Material requires 14 tokens." << endl;
						break;
					}
					Material sdfMaterial{
						instructions[2],																			// name
						{ stof(instructions[3]), stof(instructions[4]), stof(instructions[5]) },					// ka
						{ stof(instructions[6]), stof(instructions[7]), stof(instructions[8]) },					// kd
						{ stof(instructions[9]), stof(instructions[10]), stof(instructions[11]) },					// ks
						stof(instructions[12]), stof(instructions[13]) };											// reflectionExponent

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
							600																							// resolutionH
						};																						

						sdfScene.sceneCameras.emplace(instructions[2], sdfCamera);
					}

					else if (instructions.size() == 4) {
						Camera sdfCamera{ instructions[2],																// Name
										stof(instructions[3]) };														// Angle
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
			if (instructions[2] == "scale") {
				if (instructions.size() != 6) {
					cout << "Incorrect instruction syntax. Transformation scale requires 6 tokens." << endl;
				}
                sdfScene.sceneElements.at(instructions[1])->set_scaleMat(scale_vec({ stof(instructions[3]), stof(instructions[4]), stof(instructions[5]) }));

			}
			if (instructions[2] == "translate") {
				if (instructions.size() != 6) {
					cout << "Incorrect instruction syntax. Transformation translate requires 6 tokens." << endl;
				}
                sdfScene.sceneElements.at(instructions[1])->set_transMat(translate_vec({ stof(instructions[3]), stof(instructions[4]), stof(instructions[5]) }));
			}
			if (instructions[2] == "rotate") {
				if (instructions.size() != 7) {
					cout << "Incorrect instruction syntax. Transformation rotate requires 7 tokens." << endl;
				}
                sdfScene.sceneElements.at(instructions[1])->set_rotMat(rotate_vec(stof(instructions[3]) ,{stof(instructions[4]), stof(instructions[5]), stof(instructions[6]) }));
			}
            sdfScene.sceneElements.at(instructions[1])->set_world_mat();
            sdfScene.sceneElements.at(instructions[1])->set_world_inv();
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

void transormSDF(int frame){

    ofstream outfile;
    outfile.open("test.sdf");
    outfile << "# materials" << std::endl;
    outfile << "define material skin 0.6627 0.4902 0.3922 0.6627 0.4902 0.3922 0.1 0.1 0.1 10 0" << std::endl;
    outfile << "define material blue 0.2863 0.2745 0.5922 0 0 1 0.3 0.3 0.3 60 0" << std::endl;
    outfile << "define material turkis 0.0549 0.6823529 0.6823529 0.0549 0.6823529 0.6823529 0.2 0.2 0.2 5 0" << std::endl;
    outfile << "define material gturkis 0.0549 0.6823529 0.6823529 0.0549 0.6823529 0.6823529 1 1 1 10 0" << std::endl;
    outfile << "define material green 0 0.06 0 0 0.06 0 0 0 0 10 0" << std::endl;
    outfile << "define material red   1 0 0 1 0 0 1 1 1 60 0" << std::endl;
    outfile << "# geometry" << std::endl;
    outfile << "define shape box head      -20   85 -140  20  125 -100 skin" << std::endl;
    outfile << "define shape box torso     -20  -30 -140  20   80 -100 turkis" << std::endl;
    outfile << "define shape box left_arm  -45  -30 -140 -25   80 -100 skin" << std::endl;
    outfile << "define shape box right_arm  20  -30 -140  40   80 -100 skin" << std::endl;
    outfile << "define shape box right_leg   1 -145 -140  20  -35 -100 blue" << std::endl;
    outfile << "define shape box left_leg  -20 -145 -140  -1  -35 -100 blue" << std::endl;
    outfile << "define shape box ground   -1500 -155 -1500 1000 -145  100 green" << std::endl;
    outfile << "define shape sphere glitzer -170 -100 -150 45 gturkis " << std::endl;
    outfile << "define shape sphere glitzer2 170 -100 -150 45 red " << std::endl;
    outfile << "# transform" << std::endl;
    outfile << "# höhenanpassung wegen rotation" << std::endl;
    outfile << "transform right_arm translate" <<" " << 0.82*frame+3 <<" "<< 0.3*frame << " " << 0 << std::endl;
    outfile << "transform left_arm translate" <<" " << -0.82*frame+3 <<" "<< 0.3*frame << " " << 0 << std::endl;
    outfile << "# Arme heben" << std::endl;
    outfile << "transform right_arm rotate" <<" " << -0.75*frame <<" "<< 0 << " " << 0 << " " << 1 << std::endl;
    outfile << "transform left_arm rotate" <<" " << 0.75*frame <<" "<< 0 << " " << 0 << " " << 1 << std::endl;
    outfile << "# light - from right above" << std::endl;
    outfile << "define light sun 200 1000 -450 .2 .2 .2 75" << std::endl;
    outfile << "define light camlight -30 -100 120 0.1 0.1 0.1 7" << std::endl;
    outfile << "# ambient lighting" << std::endl;
    outfile << "ambient 0.5 0.5 0.5" << std::endl;
    outfile << "# camera" << std::endl;
    outfile << "define camera eye 45.0 10 40 120 -0.1 0 -1 0 1 0" << std::endl;
    outfile << "# ... and go" << std::endl;
    outfile << "render eye image.ppm 480 320" << std::endl;
    outfile << "# END -NO EMPTY LINE AFTER HERE-" << std::endl;
}

//now single threaded again
int main(int argc, char* argv[]) {
   for (int frame = 0; frame<=120; frame++){
        unsigned const image_width = 1100;
        unsigned const image_height = 850;
        string const filename = "./subdir/Steve" + std::to_string(frame) + ".ppm";


        Renderer renderer{image_width, image_height, filename};

        Scene scene;
        scene.baseLighting = {0.f, 1.f, 0.f};
        transormSDF(frame);
        parse("test.sdf", scene, renderer);


        cout << scene.sceneMaterial.size() << " Materials in scene" << endl;
        cout << scene.sceneElements.size() << " Elements in scene" << endl;
        cout << scene.lightSources.size() << " Lights in scene" << endl;
        cout << scene.sceneCameras.size() << " Cameras in scene" << endl;

        /*
        Window window{{image_width, image_height}};

        while (!window.should_close()) {
            if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                window.close();
            }
            window.show(renderer.color_buffer());
        }

        return 0;
         */
    }
}