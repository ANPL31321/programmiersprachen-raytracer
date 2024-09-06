#include "box.hpp"
#include "glm/detail/func_geometric.hpp"
#include <numbers>



#include <cmath>

Box::Box(std::string const& name, std::shared_ptr<Material> const& material, glm::vec3 const& min, glm::vec3 const& max):
	Shape::Shape{name, material},
	min_{min},
	max_{max} {}

float Box::area() const {
	glm::vec3 diag = max_ - min_;
	return (diag.x * diag.y + diag.y * diag.z + diag.z * diag.x) * 2;
}

float Box::volume() const {
	glm::vec3 diag = max_ - min_;
	return diag.x * diag.y * diag.z;
}

std::string Box::getName() const{
    return name_;
};

std::ostream& Box::print(std::ostream& os) const {
    return os << "Name: " << name_ << "\n" <<
              "Material: " << (*material_)<<
		"Min: " << "(" << min_.x << ", " << min_.y << ", " << min_.z << ")\n" <<
		"Max: " << "(" << max_.x << ", " << max_.y << ", " << max_.z << ")\n";
}

HitPoint Box::intersect(Ray const& ray_world) const {
    /*bool success = false;
    float t_min = std::numeric_limits<float>::max();
    glm::vec3 intersection_point;
    glm::vec3 normale;

    // Проверка пересечения с каждой из шести граней куба
    for (int i = 0; i < 3; ++i) {
        float t1 = (min_[i] - ray.origin[i]) / ray.direction[i];
        float t2 = (max_[i] - ray.origin[i]) / ray.direction[i];

        if (t1 > t2) std::swap(t1, t2);
        if (t1 > t_min) t_min = t1;
        if (t2 < t_min) t_min = t2;

        if (t_min < 0) continue;

        if (t1 >= 0 && t1 < t_min) {
            t_min = t1;
            intersection_point = ray.origin + t1 * ray.direction;
            normale = glm::vec3(0.0f);
            normale[i] = ray.origin[i] < min_[i] ? -1.0f : 1.0f;
            success = true;
        }
    }

    if (success) {
        return HitPoint{
                true,
                t_min,
                name_,
                material_,
                intersection_point,
                ray.direction,
                norm(normale)
        };
    } else {
        return HitPoint{ false, 0, "", nullptr, glm::vec3(), glm::vec3(), glm::vec3() };
    }*/

    bool success = false;
    Ray ray=transform_ray(ray_world);

    float t_min = -1;
    glm::vec3 normale{0.0f, 0.0f, 0.0f};
    glm::vec3 intersection_point;
    float x = min_.x;
    float t = (x - ray.origin.x) / ray.direction.x;
    float y = ray.origin.y + ray.direction.y * t;
    float z = ray.origin.z + ray.direction.z * t;
    if ((y >= min_.y && y <= max_.y) && (z >= min_.z && z <= max_.z) && t >= 0) {
        if (!success || (success && (t < t_min))) {
            t_min = t;
            intersection_point = glm::vec3{x, y, z};
            success = true;
            normale = {-1.0f, 0.0f, 0.0f};
        }
    }

    x = max_.x;
    t = (x - ray.origin.x) / ray.direction.x;
    y = ray.origin.y + ray.direction.y * t;
    z = ray.origin.z + ray.direction.z * t;
    if ((y >= min_.y && y <= max_.y) && (z >= min_.z && z <= max_.z) && t >= 0) {
        if (!success || (success && (t < t_min))) {
            t_min = t;
            intersection_point = glm::vec3{x, y, z};
            success = true;
            normale = {1.0f, 0.0f, 0.0f};
        }
    }

    y = min_.y;
    t = (y - ray.origin.y) / ray.direction.y;
    x = ray.origin.x + ray.direction.x * t;
    z = ray.origin.z + ray.direction.z * t;
    if ((x >= min_.x && x <= max_.x) && (z >= min_.z && z <= max_.z) && t >= 0) {
        if (!success || (success && (t < t_min))) {
            t_min = t;
            intersection_point = glm::vec3{x, y, z};
            success = true;
            normale = {0.0f, -1.0f, 0.0f};
        }
    }

    y = max_.y;
    t = (y - ray.origin.y) / ray.direction.y;
    x = ray.origin.x + ray.direction.x * t;
    z = ray.origin.z + ray.direction.z * t;
    if ((x >= min_.x && x <= max_.x) && (z >= min_.z && z <= max_.z) && t >= 0) {
        if (!success || (success && (t < t_min))) {
            t_min = t;
            intersection_point = glm::vec3{x, y, z};
            success = true;
            normale = {0.0f, 1.0f, 0.0f};
        }
    }

    z = min_.z;
    t = (z - ray.origin.z) / ray.direction.z;
    x = ray.origin.x + ray.direction.x * t;
    y = ray.origin.y + ray.direction.y * t;
    if ((x >= min_.x && x <= max_.x) && (y >= min_.y && y <= max_.y) && t >= 0) {
        if (!success || (success && (t < t_min))) {
            t_min = t;
            intersection_point = glm::vec3{x, y, z};
            success = true;
            normale = {0.0f, 0.0f, -1.0f};
        }
    }

    z = max_.z;
    t = (z - ray.origin.z) / ray.direction.z;
    x = ray.origin.x + ray.direction.x * t;
    y = ray.origin.y + ray.direction.y * t;
    if ((x >= min_.x && x <= max_.x) && (y >= min_.y && y <= max_.y) && t >= 0) {
        if (!success || (success && (t < t_min))) {
            t_min = t;
            intersection_point = glm::vec3{x, y, z};
            success = true;
            normale = {0.0f, 0.0f, 1.0f};
        }
    }

    if (success) {
        glm::vec4 intersection_local(intersection_point, 1.0f);
        glm::vec4 normal_local(normale, 0.0f);


        glm::vec3 intersection_world = glm::vec3(world_transformation_ * intersection_local);
        glm::vec3 normal_world = glm::normalize(glm::vec3(glm::transpose(world_transformation_) * normal_local));

        return HitPoint{true, t_min, name_, material_, intersection_world, ray_world.direction, normal_world};
    } else {
        return HitPoint{false, 0, "", nullptr, glm::vec3(), glm::vec3(), glm::vec3()};
    }}

glm::vec3 Box::getCenter() const{
    return center_;
};


void Box::translate(float const x, float const y, float const z) {
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    world_transformation_ = translation * world_transformation_;
    world_transformation_inv_ = glm::inverse(world_transformation_);
}

void Box::scale(float const x, float const y, float const z) {
    glm::mat4 scaling = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
    world_transformation_ = scaling * world_transformation_;
    world_transformation_inv_ = glm::inverse(world_transformation_);
}


void Box::rotate(float const angle, float const x, float const y, float const z) {
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(x, y, z));
    world_transformation_ = rotation * world_transformation_;
    world_transformation_inv_ = glm::inverse(world_transformation_);
}


//transform ray into local CS
Ray Box::transform_ray( Ray const& ray) const {
    glm::vec4 origin_local = world_transformation_inv_ * glm::vec4(ray.origin, 1.0f);


    glm::vec4 direction_local = world_transformation_inv_ * glm::vec4(ray.direction, 0.0f);

    return Ray{
            glm::vec3(origin_local),
            glm::normalize(glm::vec3(direction_local))
    };
}
