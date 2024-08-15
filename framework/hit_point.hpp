#ifndef HITPOINT_HPP
#define HITPOINT_HPP

#include <iostream>
#include "color.hpp"
#include <glm/vec3.hpp>
#include "material.hpp"

struct HitPoint {
	bool success;
	float distance;
	std::string name_intersected_obj;
    std::shared_ptr<Material> material_intersected_;
	glm::vec3 intersection_point;
	glm::vec3 ray_direction;
	glm::vec3 normale;
};

#endif