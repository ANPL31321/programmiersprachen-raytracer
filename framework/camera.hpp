#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <string>
#include <glm/vec3.hpp>
#include "ray.hpp"

struct Camera {
    std::string name;
    float fov_x;
    glm::vec3 eye{1.0f,1.0f,1.0f};
    glm::vec3 direction{0.0f, 0.0f, -1.0f};
    glm::vec3 up_vector{0.0f, 1.0f, 0.0f};

    // Correct declaration of the method
    Ray transform_camera_rays(const Ray& ray) const;
};

#endif // CAMERA_HPP
