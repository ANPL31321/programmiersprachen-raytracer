#include "ray.hpp"
#include <cmath>
#include <iostream>

Ray norm(Ray const& ray) {
	Ray ret_ray{ ray.origin, norm(ray.direction)};
	return ret_ray;
}

glm::vec3 norm(glm::vec3 const& vec) {
	float length = std::sqrt(vec.x * vec.x + vec.y * vec.y +
		vec.z * vec.z);
	return glm::vec3{ vec.x / length, vec.y / length, vec.z / length };
}