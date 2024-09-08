#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <iostream>
#include "color.hpp"
#include "ray.hpp"
#include "hit_point.hpp"
#include "hit_point.hpp"
#include "ray.hpp"
#include "material.hpp"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

class Shape {
public:
	Shape(std::string const& name, std::shared_ptr<Material> const& material);
	virtual float area() const = 0;
	virtual float volume() const = 0;
    virtual std::ostream& print(std::ostream& os) const;
	virtual ~Shape();
	virtual HitPoint intersect(Ray const& ray) const = 0;
    virtual void scale(float const x, float const y, float const z)=0;
    virtual void translate(float const x, float const y, float const z)=0;
    virtual void rotate(float const angle, float const x, float const y, float const z)=0;
    virtual Ray transform_ray(Ray const& ray)const=0;
    virtual std::string getName() const;
    virtual glm::vec3 getCenter()const=0;
protected:
	std::string name_;
    std::shared_ptr<Material> material_;
    glm::mat4 world_transformation_;
    glm::mat4 world_transformation_inv_;
};

std::ostream& operator<<(std::ostream& os, Shape const& s);

#endif