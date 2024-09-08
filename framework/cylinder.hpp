//
// Created by Polina on 07.09.2024.
//

#ifndef RAYTRACER_CYPINDER_HPP
#define RAYTRACER_CYPINDER_HPP
#include "shape.hpp"
#include "string"
#include "glm/glm.hpp"

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#define _USE_MATH_DEFINES
#include "glm-0.9.5.3/glm/gtx/transform.hpp"

class Cylinder:public Shape{
public:
    Cylinder(std::string const& name, std::shared_ptr<Material> const& material, glm::vec3 const& center, float radius, float hight);
    float area() const override;
    float volume() const override;
    std::string getName() const override;
    std::ostream& print(std::ostream& os) const override;
    HitPoint intersect(Ray const& ray) const override;
    void scale(float const x, float const y, float const z) override;
    void translate(float const x, float const y, float const z) override;
    void rotate(float const angle, float const x, float const y, float const z) override;
    Ray transform_ray(Ray const& ray) const override;
    glm::vec3 getCenter() const override;
private:
    float height_;
    float radius_;
    glm::vec3 center_;
    glm::mat4 world_transformation_;
    glm::mat4 world_transformation_inv_;

};


#endif //RAYTRACER_CONE_HPP
