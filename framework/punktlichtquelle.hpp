#ifndef PUNKTLICHTQUELLE_HPP
#define PUNKTLICHTQUELLE_HPP

#include <string>
#include <glm\vec3.hpp>
#include "color.hpp"

struct Punktlichquelle {
	std::string name;
	glm::vec3 position;
	Color color;
	float brightness;
};

#endif