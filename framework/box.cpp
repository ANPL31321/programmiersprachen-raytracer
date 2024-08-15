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
}