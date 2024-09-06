#include <renderer.hpp>
#include <window.hpp>

#include <GLFW/glfw3.h>
#include <thread>
#include <utility>
#include <cmath>

#include "scene.hpp"

//now single threaded again
int main(int argc, char* argv[])
{
    Scene scene{ "C:/Users/Polina/Desktop/new_folder/programmiersprachen-raytracer/source/materials.sdf" };
    unsigned const image_width = scene.get_x_res();
    unsigned const image_height = scene.get_y_res();
    std::string const filename = "./" + scene.get_output_file();

    Renderer renderer{ image_width, image_height, filename, scene };

    renderer.render();

    Window window{ {image_width, image_height} };

    while (!window.should_close()) {
        if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            window.close();
        }
        window.show(renderer.color_buffer());
    }

    return 0;
}
