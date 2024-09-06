#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "shape.hpp"
#include <glm/vec3.hpp>
#include "hit_point.hpp"
#include "ray.hpp"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#define _USE_MATH_DEFINES
#include "glm-0.9.5.3/glm/gtx/transform.hpp"

class Sphere : public Shape {
public:
	Sphere(std::string const& name, std::shared_ptr<Material> const& material, glm::vec3 const& center, float radius);
	float area() const override;
	float volume() const override;
    std::string getName();
	std::ostream& print(std::ostream& os) const override;
	HitPoint intersect(Ray const& ray) const override;
	~Sphere() override;
    virtual void scale(float const x, float const y, float const z) override;
    virtual void translate(float const x, float const y, float const z) override;
    virtual void rotate(float const angle, float const x, float const y, float const z) override;
    virtual Ray transform_ray(Ray const& ray) const override;
    glm::vec3 getCenter() const override;
private:
	glm::vec3 center_;
	float radius_;
    glm::mat4 world_transformation_;
    glm::mat4 world_transformation_inv_;
};
bool solveQuadratic(const float &a, const float &b, const float &c,
                    float &x0, float &x1);

#endif