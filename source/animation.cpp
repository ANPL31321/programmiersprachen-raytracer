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

glm::vec3 new_camera_pos(glm::vec3 const& old_camera_pos, glm::vec3 const& center, float angle_radians) {
    glm::mat4 rotation_matrix{
        glm::vec4{std::cos(angle_radians), 0.0f, std::sin(angle_radians), 0.0f},
        glm::vec4{0.0f, 1.0f, 0.0f, 0.0f},
        glm::vec4{-std::sin(angle_radians), 0.0f, std::cos(angle_radians), 0.0f},
        glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}
    };
    glm::vec4 vec_from_center_to_camera = glm::vec4{ old_camera_pos - center, 1.0f };
    vec_from_center_to_camera = rotation_matrix * vec_from_center_to_camera;
    return center + (glm::vec3) vec_from_center_to_camera;
}

glm::vec3 new_camera_dir(glm::vec3 const& center, glm::vec3 const& camera_pos) {
    return glm::normalize(center - camera_pos);
}

int main(int argc, char* argv[]) {
    Scene scene{ "C:/Anya Programming/programmiersprachen-raytracer/source/materials123.sdf" };
    //unsigned const image_width = scene.get_x_res();
    //unsigned const image_height = scene.get_y_res();
    //std::string const dir_name = "./" + scene.get_output_file();
    //scene.generate_animation_files();

    std::string objects_and_transformations;
    unsigned int image_width;
    unsigned int image_hight;
    Camera camera;

    std::string filename = "C:/Anya Programming/programmiersprachen-raytracer/source/materials123.sdf";

    std::ifstream sdf_file(filename);
    if (!sdf_file.is_open()) {
        std::cout << "Could not find or open: " << filename << std::endl;
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
            line_as_stream >> token;
            line_as_stream >> image_width;
            line_as_stream >> image_hight;
        }
        else {
            objects_and_transformations += line_buffer + "\n";
        }
    }
    
    
    std::string const dir_path = "C:/Anya Programming/test_dir_3/";
    if (!std::filesystem::create_directory(dir_path)) {
        std::filesystem::remove_all(dir_path);
        std::filesystem::create_directory(dir_path);
    }

    int amount_of_frames = 8;
    float basic_angle = 2 * std::numbers::pi / amount_of_frames;

    for (int i = 0; i < amount_of_frames; ++i) {
        std::ofstream test_file;
        test_file.open(dir_path + "image" + std::to_string(i) + ".sdf");
        test_file << objects_and_transformations;
        camera.eye = new_camera_pos(camera.eye, glm::vec3{ 0.0f, 0.0f, -100.0f }, basic_angle);
        camera.direction = new_camera_dir(glm::vec3{ 0.0f, 0.0f, -100.0f }, camera.eye);
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