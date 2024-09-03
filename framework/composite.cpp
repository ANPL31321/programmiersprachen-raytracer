//
// Created by Polina on 02.09.2024.
//
#include "composite.hpp"


Composite::Composite(std::string const& name, std::shared_ptr<Material> const& material)
        : Shape::Shape{name, material},
          world_transformation_{glm::mat4(1.0f)},
          world_transformation_inv_{glm::mat4(1.0f)} {

}



void Composite::add(std::shared_ptr<Shape> shape) {
    children_.push_back(shape);

}

void Composite::remove(std::shared_ptr<Shape> shape) {
    auto it = std::find(children_.begin(), children_.end(), shape);
    if (it != children_.end()) {
        children_.erase(it);
    }

}

float Composite::volume() const {
    float total_volume = 0.0f;
    for (const auto& child : children_)
    {total_volume += child->volume();}
    return total_volume;
}

std::ostream& Composite::print(std::ostream& os) const  {
    os << "Composite: " << name_ << " with " << children_.size() << " children." << std::endl;
    for (const auto& child : children_) {
    child->print(os);
    }
    return os;
}

float Composite::area() const {
    float total_area = 0.0f;
    for (const auto& child : children_) {
        total_area += child->area();
    }

    return total_area;
}

HitPoint Composite::intersect(Ray const& ray) const {
    HitPoint closest_hit;
    closest_hit.distance = std::numeric_limits<float>::max();
    closest_hit.success = false;

    for (const auto& child : children_) {
        HitPoint hit_point = child->intersect(ray);
        if (hit_point.success && hit_point.distance < closest_hit.distance) {
            closest_hit = hit_point;
        }
    }

    return closest_hit;
}

void Composite::scale(float x, float y, float z) {
    for (auto& child : children_) {
        child->scale(x, y, z);
    }

}


void Composite::translate(float x, float y, float z) {
    for (auto& child : children_) {
        child->translate(x, y, z);
    }

}

void Composite::rotate(float angle, float x, float y, float z) {
    for (auto& child : children_) {
        child->rotate(angle, x, y, z);

    }
}

Ray Composite::transform_ray(Ray const& ray) const {
    std::cout<<"ray transformation"<<std::endl;
    if (!children_.empty()) {
        return (children_.front())->transform_ray(ray);
    }else return ray;
}

std::string Composite::getName() const {
    return name_;
}


glm::vec3 Composite::getCenter() const {
    if (children_.empty()) return glm::vec3(0.0f);


    glm::vec3 center(0.0f);
    for (const auto& child : children_) {
        center += child->getCenter();
    }
    return center / static_cast<float>(children_.size());
}
