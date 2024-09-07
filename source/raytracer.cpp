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
    std::string const dir_path = "C:/Anya Programming/test_dir_2/";
    for (auto const& file : std::filesystem::directory_iterator{ dir_path }) {
        //std::string const filepath = file.path().string();
        Scene scene{ file.path().string()};
        unsigned const image_width = scene.get_x_res();
        unsigned const image_height = scene.get_y_res();
        std::filesystem::create_directory("../rendered_test_2/");
        std::string const filename = "../rendered_test_2/" + scene.get_output_file();

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
