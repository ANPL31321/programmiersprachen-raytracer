#include <renderer.hpp>
#include <window.hpp>

#include <GLFW/glfw3.h>
#include <thread>
#include <utility>
#include <cmath>

#include "scene.hpp"
#include <filesystem>

//now single threaded again
int main(int argc, char* argv[])
{
    std::string const dir_path_input = "C:/Users/LEGION/OneDrive/Documents/Uni/Software Engeneering/ray_tracer_tests/camera_moves_forward.ppm/";
    std::string const dir_path_output = dir_path_input + "rendered_frames/";
    if (!std::filesystem::create_directory(dir_path_output)) {
        std::filesystem::remove_all(dir_path_output);
        std::filesystem::create_directory(dir_path_output);
    }
    for (auto const& file : std::filesystem::directory_iterator{ dir_path_input }) {
        //std::string const filepath = file.path().string();
        if (file.is_directory()) {
            continue;
        }
        Scene scene{ file.path().string()};
        unsigned const image_width = scene.get_x_res();
        unsigned const image_height = scene.get_y_res();
        std::string const filename = dir_path_output + scene.get_output_file();

        Renderer renderer{ image_width, image_height, filename, scene };

        renderer.render();

        /*Window window{{image_width, image_height}};

        while (!window.should_close()) {
            if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                window.close();
            }
            window.show(renderer.color_buffer());
        }*/
    }

    return 0;
}
