//
// Created by Polina on 01.09.2024.
//
#include "camera.hpp"
#include <glm/glm.hpp>
#include "ray.hpp"
#include "glm/detail/func_geometric.hpp"
#include "glm-0.9.5.3/glm/gtx/transform.hpp"

Ray Camera::transform_camera_rays(Ray const& ray) const {
    // Compute basis vectors
    glm::vec3 right = glm::normalize(glm::cross(up_vector, direction));
    glm::vec3 up = glm::normalize(glm::cross(direction, right));
    glm::vec3 forward = glm::normalize(direction);

    // Construct the transformation matrix
    glm::mat4 mat4{
            glm::vec4(right, 0.0f),
            glm::vec4(up, 0.0f),
            glm::vec4(-forward, 0.0f),
            glm::vec4(eye, 1.0f)
    };

    glm::vec4 origin_in_HCS{ray.origin, 1.0f};
    glm::vec4 direction_in_HCS{ray.direction, 0.0f};

    glm::vec3 new_origin{mat4 * origin_in_HCS};
    glm::vec3 new_direction{mat4 * direction_in_HCS};

    return Ray{new_origin, glm::normalize(new_direction)};
}

void Camera::move_camera(float const rotation_angle) {
    eye += direction;
    glm::mat4 rotation_matrix{
        glm::vec4{std::cos(rotation_angle), -std::sin(rotation_angle), 0.0f, 0.0f},
        glm::vec4{std::sin(rotation_angle), std::cos(rotation_angle), 0.0f, 0.0f},
        glm::vec4{0.0f, 0.0f, 1.0f, 0.0f},
        glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}
    };
    up_vector = (glm::vec3)(rotation_matrix * glm::vec4{ up_vector, 1.0f });
}