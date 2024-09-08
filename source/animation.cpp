#include <renderer.hpp>
#include <window.hpp>

#include <GLFW/glfw3.h>
#include <thread>
#include <utility>
#include <cmath>

#include "scene.hpp"

#include <filesystem>
#include <fstream>

#include <glm/glm.hpp>
#include <numbers>

int main(int argc, char* argv[]) {
    std::filesystem::path const filepath = "C:/Users/LEGION/OneDrive/Documents/Uni/Software Engeneering/ray_tracer_tests/materials123.sdf";

    std::string objects_and_transformations;
    unsigned int image_width;
    unsigned int image_hight;
    std::string output_file;
    Camera camera;;

    std::ifstream sdf_file(filepath);
    if (!sdf_file.is_open()) {
        std::cout << "Could not find or open: " << filepath << std::endl;
        return 0;
    }

    std::string line_buffer;
    while (std::getline(sdf_file, line_buffer)) {
        std::istringstream line_as_stream(line_buffer);
        std::string token;
        line_as_stream >> token;
        if ("define" == token) {
            line_as_stream >> token;
            if ("camera" == token) {
                line_as_stream >> camera.name;
                line_as_stream >> camera.fov_x;
                line_as_stream >> camera.eye.x >> camera.eye.y >> camera.eye.z;
                line_as_stream >> camera.direction.x >> camera.direction.y >> camera.direction.z;
                line_as_stream >> camera.up_vector.x >> camera.up_vector.y >> camera.up_vector.z;
            }
            else {
                objects_and_transformations += line_buffer + "\n";
            }
        }
        else if ("render" == token) {
            line_as_stream >> token;
            line_as_stream >> output_file;
            line_as_stream >> image_width;
            line_as_stream >> image_hight;
        }
        else {
            objects_and_transformations += line_buffer + "\n";
        }
    }
    
    
    std::string const dir_path = filepath.parent_path().string() + "/" + output_file + "/"; // LoD
    if (!std::filesystem::create_directory(dir_path)) {
        std::filesystem::remove_all(dir_path);
        std::filesystem::create_directory(dir_path);
    }

    int amount_of_frames = 4;
    float rotation_angle = 2 * std::numbers::pi / amount_of_frames;

    for (int i = 0; i < amount_of_frames; ++i) {
        std::ofstream test_file;
        test_file.open(dir_path + "image" + std::to_string(i) + ".sdf");
        test_file << objects_and_transformations;
        camera.move_camera(rotation_angle);
        test_file << "define camera " << camera.name << " " << camera.fov_x << " " <<
            camera.eye.x << " " << camera.eye.y << " " << camera.eye.z << " " <<
            camera.direction.x << " " << camera.direction.y << " " << camera.direction.z << " " << 
            camera.up_vector.x << " " << camera.up_vector.y << " " << camera.up_vector.z << "\n";
        test_file << "render " << camera.name << " image" << i << ".ppm " <<
            image_width << " " << image_hight << "\n";
        test_file.close();
    }

    return 0;
}