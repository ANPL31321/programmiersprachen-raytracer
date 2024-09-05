#ifndef BOX_HPP
#define BOX_HPP

#include "shape.hpp"
#include <glm/vec3.hpp>
#include "hit_point.hpp"
#include "ray.hpp"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#define _USE_MATH_DEFINES
#include "glm-0.9.5.3/glm/gtx/transform.hpp"

class Box : public Shape {
public:
	Box(std::string const& name, std::shared_ptr<Material> const& material, glm::vec3 const& min, glm::vec3 const& max);
	float area() const override;
	float volume() const override;
    std::string getName() const;
	std::ostream& print(std::ostream& os) const override;
	HitPoint intersect(Ray const& ray) const override;
    void scale(float const x, float const y, float const z) override;
    void translate(float const x, float const y, float const z) override;
    void rotate(float const angle, float const x, float const y, float const z) override;
    Ray transform_ray(Ray const& ray) const;
    glm::vec3 getCenter() const override;
private:
    glm::vec3 center_;
	glm::vec3 min_;
	glm::vec3 max_;
    glm::mat4 world_transformation_;
    glm::mat4 world_transformation_inv_;
};


#endif