//
// Created by Polina on 07.09.2024.
//
#include <iostream>
#include "shape.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <limits>
#include <memory>
#include "triangle.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <cmath>
#include <limits>
#include <memory>
#include <string>


Triangle::Triangle(const std::string& name, std::shared_ptr<Material> material,
                   const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
        : Shape{name, material}, v0_{v0}, v1_{v1}, v2_{v2} {}


float Triangle::area() const {
    return 0.5f * glm::length(glm::cross(v1_ - v0_, v2_ - v0_));
}


float Triangle::volume() const {
    return 0.0f;
}

std::string Triangle::getName() const {
    return name_;
}

std::ostream& Triangle::print(std::ostream& os) const {
    os << "Triangle: " << name_ << "\n"
       << "Vertices: " << glm::to_string(v0_) << ", "
       << glm::to_string(v1_) << ", " << glm::to_string(v2_) << "\n";
    return os;
}

Ray Triangle::transform_ray(const Ray& ray) const {
    glm::vec3 origin = glm::vec3(world_transformation_inv_ * glm::vec4(ray.origin, 1.0f));
    glm::vec3 direction = glm::normalize(glm::vec3(world_transformation_inv_ * glm::vec4(ray.direction, 0.0f)));
    return {origin, direction};
}

glm::vec3 Triangle::getCenter() const {
    return (v0_ + v1_ + v2_) / 3.0f;
}

HitPoint Triangle::intersect(const Ray& ray) const {
    Ray local_ray = transform_ray(ray);

    glm::vec3 edge1 = v1_ - v0_;
    glm::vec3 edge2 = v2_ - v0_;
    glm::vec3 h = glm::cross(local_ray.direction, edge2);
    float a = glm::dot(edge1, h);

    if (std::fabs(a) < std::numeric_limits<float>::epsilon()) {
        return {false, std::numeric_limits<float>::infinity(), name_, nullptr, {}, {}, {}};
    }

    float f = 1.0f / a;
    glm::vec3 s = local_ray.origin - v0_;
    float u = f * glm::dot(s, h);

    if (u < 0.0f || u > 1.0f) {
        return {false, std::numeric_limits<float>::infinity(), name_, nullptr, {}, {}, {}};
    }

    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(local_ray.direction, q);

    if (v < 0.0f || u + v > 1.0f) {
        return {false, std::numeric_limits<float>::infinity(), name_, nullptr, {}, {}, {}};
    }

    float t = f * glm::dot(edge2, q);

    if (t < 0.0f) {
        return {false, std::numeric_limits<float>::infinity(), name_, nullptr, {}, {}, {}};
    }

    glm::vec3 intersection_point_local = local_ray.origin + t * local_ray.direction;
    glm::vec3 intersection_point = glm::vec3(world_transformation_ * glm::vec4(intersection_point_local, 1.0f));

    glm::vec3 normal_local = glm::normalize(glm::cross(edge1, edge2));
    glm::vec3 normal_world = glm::normalize(glm::vec3(glm::transpose(world_transformation_inv_) * glm::vec4(normal_local, 0.0f)));

    return {true, t, name_, material_, intersection_point, ray.direction, normal_world};
}

void Triangle::scale(float x, float y, float z) {
    glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
    world_transformation_ = world_transformation_ * scale_matrix;
    world_transformation_inv_ = glm::inverse(world_transformation_);
}

void Triangle::translate(float x, float y, float z) {
    glm::mat4 translate_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    world_transformation_ = world_transformation_ * translate_matrix;
    world_transformation_inv_ = glm::inverse(world_transformation_);
}

void Triangle::rotate(float angle, float x, float y, float z) {
    glm::mat4 rotate_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(x, y, z));
    world_transformation_ = world_transformation_ * rotate_matrix;
    world_transformation_inv_ = glm::inverse(world_transformation_);
}
