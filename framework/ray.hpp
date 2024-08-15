#ifndef RAY_HPP
#define RAY_HPP

#include <glm\vec3.hpp>

struct Ray {
	glm::vec3 origin = { 0.0f, 0.0f, 0.0f };
	glm::vec3 direction = { 0.0f, 0.0f, 0.0f };
};

Ray norm(Ray const& ray);
glm::vec3 norm(glm::vec3 const& vec);

#endif