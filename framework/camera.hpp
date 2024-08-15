#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <string>
#include <glm\vec3.hpp>

struct Camera {
	std::string name;
	float fov_x;
	glm::vec3 positon{ 0.0f, 0.0f, 0.0f };
	glm::vec3 direction{ 0.0f, 0.0f, -1.0f };
};

#endif