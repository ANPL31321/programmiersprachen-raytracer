//
// Created by Polina on 07.09.2024.
//

#include "cylinder.hpp"
#include <iostream>
#include <cmath>
#include <limits>
#define _USE_MATH_DEFINES
#include <math.h>
#include "glm/gtx/intersect.hpp"
#include "glm/glm.hpp"
#include "glm/detail/func_geometric.hpp"
#include "gl/glew.h"

Cylinder::Cylinder(std::string const& name, std::shared_ptr<Material> const& material, glm::vec3 const& center, float radius, float height)
        : Shape{name, material}, center_{center}, radius_{radius}, height_{height}, world_transformation_(1.0f), world_transformation_inv_(1.0f) {}


float Cylinder::area() const {
    return 2.0f * M_PI * radius_ * (radius_ + height_);
}


float Cylinder::volume() const {
    return M_PI * radius_ * radius_ * height_;
}


std::string Cylinder::getName() const{
    return name_;
}


std::ostream& Cylinder::print(std::ostream& os) const {
    return os << "Name: " << name_ << "\n"
              << "Material: " << (*material_) << "\n"
              << "Center: (" << center_.x << ", " << center_.y << ", " << center_.z << ")\n"
              << "Radius: " << radius_ << "\n"
              << "Height: " << height_ << "\n";
}


HitPoint Cylinder::intersect(const Ray& world_ray) const {

    Ray ray = transform_ray(world_ray);
    glm::vec3 pos = ray.origin;
    glm::vec3 dir = glm::normalize(ray.direction);

    float a = dir.x * dir.x + dir.z * dir.z;
    float b = 2 * (pos.x * dir.x + pos.z * dir.z - center_.x * dir.x - center_.z * dir.z);
    float c = (pos.x - center_.x) * (pos.x - center_.x) + (pos.z - center_.z) * (pos.z - center_.z) - radius_ * radius_;


    float discriminant = b * b - 4 * a * c;

    float t_side = std::numeric_limits<float>::infinity();
    if (discriminant >= 0.0f) {

        float sqrt_discriminant = sqrt(discriminant);
        float t1 = (-b - sqrt_discriminant) / (2 * a);
        float t2 = (-b + sqrt_discriminant) / (2 * a);

        glm::vec3 p1 = pos + t1 * dir;
        glm::vec3 p2 = pos + t2 * dir;

        if (t1 > 0 && p1.y >= center_.y && p1.y <= center_.y + height_) {
            t_side = t1;
        }
        if (t2 > 0 && p2.y >= center_.y && p2.y <= center_.y + height_ && t2 < t_side) {
            t_side = t2;
        }
    }

    float t_bottom = (center_.y - pos.y) / dir.y;
    glm::vec3 p_bottom = pos + t_bottom * dir;
    if (t_bottom > 0 && glm::length(glm::vec2(p_bottom.x - center_.x, p_bottom.z - center_.z)) <= radius_) {
        t_side = std::min(t_side, t_bottom);
    }

    float t_top = (center_.y + height_ - pos.y) / dir.y;
    glm::vec3 p_top = pos + t_top * dir;
    if (t_top > 0 && glm::length(glm::vec2(p_top.x - center_.x, p_top.z - center_.z)) <= radius_) {
        t_side = std::min(t_side, t_top);
    }

    if (t_side == std::numeric_limits<float>::infinity()) {
        return HitPoint{false};
    }

    glm::vec3 intersection_point = pos + t_side * dir;
    glm::vec3 normal;

    if (t_side == t_bottom) {
        normal = glm::vec3(0, -1, 0);
    } else if (t_side == t_top) {
        normal = glm::vec3(0, 1, 0);
    }

    return HitPoint{true, t_side, "Cylinder", material_, intersection_point, dir, normal};
}

void Cylinder::scale(float const x, float const y, float const z) {
    height_*=y;
    radius_*=(x+y)/2;
}

void Cylinder::translate(float const x, float const y, float const z) {
    center_.x += x;
    center_.y += y;
    center_.z += z;
}

void Cylinder::rotate(float const angle, float const x, float const y, float const z) {
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(x, y, z));
    world_transformation_ = rotation * world_transformation_;
    world_transformation_inv_ = glm::inverse(world_transformation_);
}

Ray Cylinder::transform_ray(Ray const& ray) const {
    glm::vec4 origin_local = world_transformation_inv_ * glm::vec4(ray.origin, 1.0f);
    glm::vec4 direction_local = world_transformation_inv_ * glm::vec4(ray.direction, 0.0f);
    return Ray{glm::vec3(origin_local), glm::normalize(glm::vec3(direction_local))};
}

glm::vec3 Cylinder::getCenter() const {
    return center_;
}

