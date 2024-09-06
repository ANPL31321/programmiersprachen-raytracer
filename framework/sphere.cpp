#include "sphere.hpp"
#include <numbers>
#include "glm/gtx/intersect.hpp"

Sphere::Sphere(std::string const& name, std::shared_ptr<Material> const& material, glm::vec3 const& center, float radius):
	Shape::Shape{name, material},
	center_{center},
	radius_{radius} {
	//std::cout << "Sphere Konstruktor Aufruf" << std::endl;
}

float Sphere::area() const {
	return 4 * std::numbers::pi * radius_ * radius_ ;
}

float Sphere::volume() const {
	return (4.0f / 3.0f) * std::numbers::pi * radius_ * radius_ * radius_;
}

glm::vec3 Sphere::getCenter() const{
    return center_;
};

std::ostream& Sphere::print(std::ostream& os) const {
    return os << "Name: " << name_ << "\n" <<
              "Material: " << (*material_)<<
		"Radius: " << radius_ << "\n" <<
		"Center: " << "(" << center_.x << ", " << center_.y << ", " << center_.z << ")\n";
}

std::string Sphere::getName() {
    return name_;
}

void Sphere::scale(float const x, float const y, float const z) {
    float scaleFactor = (x + y + z) / 3.0f;
    radius_*= scaleFactor;
    std::cout << "Sphere scaled to radius " << radius_ << std::endl;
}
void Sphere::translate(float const x, float const y, float const z) {
    center_.x += x;
    center_.y += y;
    center_.z += z;
    std::cout << "Sphere moved to (" << center_.x << ", " << center_.y << ", " << center_.z << ")" << std::endl;
}
void Sphere::rotate(float const angle, float const x, float const y, float const z) {
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(x, y, z));
    world_transformation_ = rotation * world_transformation_;
    world_transformation_inv_ = glm::inverse(world_transformation_);
}
Ray Sphere::transform_ray(Ray const& ray)const{
    // Преобразование начальной точки луча в локальную систему координат
    glm::vec4 origin_local = world_transformation_inv_ * glm::vec4(ray.origin, 1.0f);

    // Преобразование направления луча в локальную систему координат
    glm::vec4 direction_local = world_transformation_inv_ * glm::vec4(ray.direction, 0.0f);

    return Ray{
            glm::vec3(origin_local),
            glm::normalize(glm::vec3(direction_local))
    };
};


HitPoint Sphere::intersect(Ray const& ray_world) const {

    Ray ray=transform_ray(ray_world);
    HitPoint hit_point{ false, 0.0f, Shape::name_, Shape::material_, {0.0f, 0.0f, 0.0f}, ray.direction, {0.0f, 0.0f, 0.0f} };

    float t0, t1;

    glm::vec3 L = ray.origin - center_;

    float a = glm::dot(ray.direction, ray.direction);
    float b = 2 * glm::dot(ray.direction, L);
    float c = glm::dot(L, L) - radius_ * radius_;


    if (!solveQuadratic(a, b, c, t0, t1)) {
        // If there is no intersections return false
        return hit_point;
    }

    // swap if roots are in wrong order
    if (t0 > t1) std::swap(t0, t1);

    // check if there is intersect behind ray's origin
    if (t0 < 0) {
        t0 = t1; // if t0<0 then use t1
        if (t0 < 0) {
            // if t1<0 there is no intersection
            return hit_point;
        }
    }

    float t = t0;
    hit_point.success = true;
    hit_point.distance = t;


    glm::vec3 intersection_point = ray.origin + t * ray.direction;
    hit_point.intersection_point = intersection_point;

    hit_point.normale = glm::normalize(intersection_point - center_);

    if (hit_point.success) {
        glm::vec4 intersection_local(intersection_point, 1.0f);
        glm::vec4 normal_local(hit_point.normale, 0.0f);

        // transform normale back into worlds coordinates
        glm::vec3 intersection_world = glm::vec3(world_transformation_ * intersection_local);
        glm::vec3 normal_world = glm::normalize(glm::vec3(glm::transpose(world_transformation_inv_) * normal_local));

        return HitPoint{true, t, name_, material_, intersection_world, ray_world.direction, normal_world};
    } else {
        return HitPoint{false, 0, "", nullptr, glm::vec3(), glm::vec3(), glm::vec3()};
    }
}

bool solveQuadratic(const float &a, const float &b, const float &c,
                    float &x0, float &x1) {
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = - 0.5 * b / a;
    else {
        float q = (b > 0) ?
                  -0.5 * (b + sqrt(discr)) :
                  -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);

    return true;
}

Sphere::~Sphere() {
	std::cout << "Sphere Destruktor Aufruf" << std::endl;
}
