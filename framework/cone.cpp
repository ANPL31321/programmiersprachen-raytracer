//
// Created by Polina on 07.09.2024.
//
// Cone.cpp
#include "cone.hpp"
#include <numbers>
#define _USE_MATH_DEFINES
#include <math.h>
#include <numbers>
#include "glm/gtx/intersect.hpp"

Cone::Cone(std::string const& name, std::shared_ptr<Material> const& material, glm::vec3 const& center, float height, float radius)
        : Shape{name, material}, center_{center}, height_{height}, radius_{radius} {}

float Cone::area() const {
    float slant_height = sqrt((radius_ * radius_) + (height_ * height_));
    float lateral_area = M_PI * radius_ * slant_height;
    float base_area = M_PI * radius_ * radius_;
    return lateral_area + base_area;
}



HitPoint Cone::intersect(const Ray& world_ray) const {
    Ray local_ray = transform_ray(world_ray);
    glm::vec3 pos = local_ray.origin;
    glm::vec3 dir = local_ray.direction;

    float A = pos.x - center_.x;
    float B = pos.z - center_.z;
    float D = height_ - pos.y + center_.y;

    float tan = (radius_ / height_) * (radius_ / height_);

    float a = (dir.x * dir.x) + (dir.z * dir.z) - (tan * (dir.y * dir.y));
    float b = (2 * A * dir.x) + (2 * B * dir.z) + (2 * tan * D * dir.y);
    float c = (A * A) + (B * B) - (tan * (D * D));

    float discr = b * b - 4 * a * c;
    if (fabs(discr) < 0.001 || discr < 0.0f) {
        return HitPoint{false};
    }

    float t1 = (-b - sqrt(discr)) / (2 * a);
    float t2 = (-b + sqrt(discr)) / (2 * a);
    float t = (t1 > t2) ? t2 : t1;

    glm::vec3 intersection_point_local = pos + t * dir;
    float r = intersection_point_local.y;

    if (r > center_.y && r < center_.y + height_) {
        glm::vec3 normal_local = Cone::normal(intersection_point_local);
        glm::vec3 intersection_point_world = glm::vec3(world_transformation_ * glm::vec4(intersection_point_local, 1.0f));
        glm::vec3 normal_world = glm::normalize(glm::vec3(glm::transpose(world_transformation_inv_) * glm::vec4(normal_local, 0.0f)));

        return HitPoint{
                true,
                t,
                name_,
                material_,
                intersection_point_world,
                world_ray.direction,
                normal_world
        };
    }

    return HitPoint{false};
}

glm::vec3 Cone::normal(const glm::vec3& p) const {
    // calculate gradient
    float r = sqrt((p.x - center_.x) * (p.x - center_.x) + (p.z - center_.z) * (p.z - center_.z));
    glm::vec3 n = glm::vec3(p.x - center_.x, r * (radius_ / height_), p.z - center_.z);
    return glm::normalize(n);
}


float Cone::volume() const {
    return (1.0f / 3.0f) * M_PI * radius_ * radius_ * height_;
}

glm::vec3 Cone::getCenter() const {
    return center_;
}

std::string Cone::getName()  const{
    return name_;
}

std::ostream& Cone::print(std::ostream& os) const {
    return os << "Name: " << name_ << "\n"
              << "Material: " << (*material_) << "\n"
              << "Base: (" << center_.x << ", " << center_.y << ", " << center_.z << ")\n"
              << "Height: " << height_ << "\n"
              << "Radius: " << radius_ << "\n";
}



void Cone::scale(float const x, float const y, float const z) {
    float scaleFactor = (x + y + z) / 3.0f;
    radius_*= scaleFactor;
    height_*= scaleFactor;
}


void Cone::translate(float const x, float const y, float const z) {
    center_.x += x;
    center_.y += y;
    center_.z += z;
}

void Cone::rotate(float const angle, float const x, float const y, float const z) {
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(x, y, z));
    world_transformation_ = rotation * world_transformation_;
    world_transformation_inv_ = glm::inverse(world_transformation_);
}

Ray Cone::transform_ray(Ray const& ray)const{
    glm::vec4 origin_local = world_transformation_inv_ * glm::vec4(ray.origin, 1.0f);
    glm::vec4 direction_local = world_transformation_inv_ * glm::vec4(ray.direction, 0.0f);

    return Ray{
            glm::vec3(origin_local),
            glm::normalize(glm::vec3(direction_local))
    };
};