#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "color.hpp"

class Shape {
public:
	Shape(std::string name, Color color);
	virtual float area() const = 0;
	virtual float volume() const = 0;
private:
	std::string name_;
	Color color_;
};

#endif