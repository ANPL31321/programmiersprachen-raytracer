#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <iostream>
#include "color.hpp"
#include "ray.hpp"
#include "hit_point.hpp"
#include "hit_point.hpp"
#include "ray.hpp"
#include "material.hpp"

class Shape {
public:
	Shape(std::string const& name, std::shared_ptr<Material> const& material);
	virtual float area() const = 0;
	virtual float volume() const = 0;
	virtual std::ostream& print(std::ostream& os) const;
	virtual ~Shape();
	virtual HitPoint intersect(Ray const& ray) const = 0;
	std::string const& get_name() const;
protected:
	std::string name_;
    std::shared_ptr<Material> material_;
};

std::ostream& operator<<(std::ostream& os, Shape const& s);

#endif