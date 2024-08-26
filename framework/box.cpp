#include "box.hpp"
#include <numbers>

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

std::ostream& Box::print(std::ostream& os) const {
	return Shape::print(os) <<
		"Min: " << "(" << min_.x << ", " << min_.y << ", " << min_.z << ")\n" <<
		"Max: " << "(" << max_.x << ", " << max_.y << ", " << max_.z << ")\n";
}

HitPoint Box::intersect(Ray const& ray) const {
    /*
    bool success = false;
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
           intersection_point = glm::vec3{ x, y, z };
           success = true;
           normale = { -1.0f, 0.0f, 0.0f };
       }
   }

   x = max_.x;
   t = (x - ray.origin.x) / ray.direction.x;
   y = ray.origin.y + ray.direction.y * t;
   z = ray.origin.z + ray.direction.z * t;
   if ((y >= min_.y && y <= max_.y) && (z >= min_.z && z <= max_.z) && t >= 0) {
       if (!success || (success && (t < t_min))) {
           t_min = t;
           intersection_point = glm::vec3{ x, y, z };
           success = true;
           normale = { 1.0f, 0.0f, 0.0f };
       }
   }

   y = min_.y;
   t = (y - ray.origin.y) / ray.direction.y;
   x = ray.origin.x + ray.direction.x * t;
   z = ray.origin.z + ray.direction.z * t;
   if ((x >= min_.x && x <= max_.x) && (z >= min_.z && z <= max_.z) && t >= 0) {
       if (!success || (success && (t < t_min))) {
           t_min = t;
           intersection_point = glm::vec3{ x, y, z };
           success = true;
           normale = { 0.0f, -1.0f, 0.0f };
       }
   }

   y = max_.y;
   t = (y - ray.origin.y) / ray.direction.y;
   x = ray.origin.x + ray.direction.x * t;
   z = ray.origin.z + ray.direction.z * t;
   if ((x >= min_.x && x <= max_.x) && (z >= min_.z && z <= max_.z) && t >= 0) {
       if (!success || (success && (t < t_min))) {
           t_min = t;
           intersection_point = glm::vec3{ x, y, z };
           success = true;
           normale = { 0.0f, 1.0f, 0.0f };
       }
   }

   z = min_.z;
   t = (z - ray.origin.z) / ray.direction.z;
   x = ray.origin.x + ray.direction.x * t;
   y = ray.origin.y + ray.direction.y * t;
   if ((x >= min_.x && x <= max_.x) && (y >= min_.y && y <= max_.y) && t >= 0) {
       if (!success || (success && (t < t_min))) {
           t_min = t;
           intersection_point = glm::vec3{ x, y, z };
           success = true;
           normale = { 0.0f, 0.0f, -1.0f };
       }
   }

   z = max_.z;
   t = (z - ray.origin.z) / ray.direction.z;
   x = ray.origin.x + ray.direction.x * t;
   y = ray.origin.y + ray.direction.y * t;
   if ((x >= min_.x && x <= max_.x) && (y >= min_.y && y <= max_.y) && t >= 0) {
       if (!success || (success && (t < t_min))) {
           t_min = t;
           intersection_point = glm::vec3{ x, y, z };
           success = true;
           normale = { 0.0f, 0.0f, 1.0f };
       }
   }

    return HitPoint { success, t_min, Shape::name_, Shape::material_, intersection_point, ray.direction, normale };
    /*HitPoint hitPoint{false, 0, Shape::name_, Shape::material_, {0,0,0}, ray.direction, {0,0,0} };
    float tmin = (min_.x - ray.origin.x) / ray.direction.x;
    float tmax = (max_.x - ray.origin.x) / ray.direction.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (min_.y - ray.origin.y) / ray.direction.y;
    float tymax = (max_.y - ray.origin.y) / ray.direction.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return hitPoint;

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    float tzmin = (min_.z - ray.origin.z) / ray.direction.z;
    float tzmax = (max_.z - ray.origin.z) / ray.direction.z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return hitPoint;

    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;
    success= true;

    glm::vec3 point = ray.direction*tmin;
    glm::vec3 normal;
    if (std::abs(point.x) > std::abs(point.y) && std::abs(point.x) > std::abs(point.z)) {
        normal= glm::vec3(point.x > 0 ? 1.0f : -1.0f, 0.0f, 0.0f);
    } else if (std::abs(point.y) > std::abs(point.x) && std::abs(point.y) > std::abs(point.z)) {
        normal= glm::vec3(0.0f, point.y > 0 ? 1.0f : -1.0f, 0.0f);
    } else {
        normal = glm::vec3(0.0f, 0.0f, point.z > 0 ? 1.0f : -1.0f);
    }

    hitPoint={success,tmin,Shape::name_, Shape::material_,ray.direction*tmin, ray.direction, normal };*/
}