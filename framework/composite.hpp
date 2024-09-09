//
// Created by Polina on 02.09.2024.
//

#ifndef RAYTRACER_COMPOSITE_HPP
#define RAYTRACER_COMPOSITE_HPP

#include "shape.hpp"
#include "vector"

class Composite:public Shape{
public:
    Composite(std::string const& name, std::shared_ptr<Material> const& material);
    float area() const override;
    float volume() const override;
    std::ostream& print(std::ostream& os) const;
    HitPoint intersect(Ray const& ray) const override;
    void scale(float const x, float const y, float const z) override;
    void translate(float const x, float const y, float const z) override;
    void rotate(float const angle, float const x, float const y, float const z) override;
    Ray transform_ray(Ray const& ray)const override;
    glm::vec3 getCenter()const override;
    void add(std::shared_ptr<Shape> shape);
    void remove(std::shared_ptr<Shape> shape);
    std::vector<std::shared_ptr<Shape>> getChildren();
protected:
    std::string name_;
    std::shared_ptr<Material> material_;
    glm::mat4 world_transformation_;
    glm::mat4 world_transformation_inv_;
    std::vector<std::shared_ptr<Shape>> children_;
};

std::ostream& operator<<(std::ostream& os, Shape const& s);


#endif //RAYTRACER_COMPOSITE_HPP
