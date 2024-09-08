//
// Created by Polina on 07.09.2024.
//

#ifndef RAYTRACER_TRIANGLE_HPP
#define RAYTRACER_TRIANGLE_HPP
#include "shape.hpp"
#include "string"

class Triangle:public Shape{
public:
    Triangle(const std::string& name, std::shared_ptr<Material> material,
             const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
    float area() const override;
    float volume() const override;
    std::string getName() const override;
    std::ostream& print(std::ostream& os) const override;
    HitPoint intersect(Ray const& ray) const override;
    virtual void scale(float const x, float const y, float const z) override;
    void translate(float const x, float const y, float const z) override;
    void rotate(float const angle, float const x, float const y, float const z) override;
    Ray transform_ray(Ray const& ray) const override;
    glm::vec3 getCenter() const override;
private:
    glm::vec3 center_,v0_,v1_,v2_;
    glm::mat4 world_transformation_;
    glm::mat4 world_transformation_inv_;
};


#endif //RAYTRACER_TRIANGLE_HPP
