#include "sphere.hpp"
#include <numbers>
#include "glm/gtx/intersect.hpp"

Sphere::Sphere(std::string const& name, std::shared_ptr<Material> const& material, glm::vec3 const& center, float radius):
	Shape::Shape{name, material},
	center_{center},
	radius_{radius} {
	std::cout << "Sphere Konstruktor Aufruf" << std::endl;
}

float Sphere::area() const {
	return 4 * std::numbers::pi * radius_ * radius_ ;
}

float Sphere::volume() const {
	return (4.0f / 3.0f) * std::numbers::pi * radius_ * radius_ * radius_;
}

std::ostream& Sphere::print(std::ostream& os) const {
	return Shape::print(os) <<
		"Radius: " << radius_ << "\n" <<
		"Center: " << "(" << center_.x << ", " << center_.y << ", " << center_.z << ")\n";
}

HitPoint Sphere::intersect(Ray const& ray) const {
	HitPoint hit_point{ false, 0.0f, Shape::name_, Shape::material_, {0.0f, 0.0f, 0.0f}, ray.direction, {0.0f, 0.0f, 0.0f} };
	/*hit_point.success = glm::intersectRaySphere(ray.origin, ray.direction, center_,
		radius_ * radius_, hit_point.distance);*/

    float t1,t0,t;
    glm::vec3 L = ray.origin - center_;
    float a = glm::dot(ray.direction,ray.direction);
    float b = 2*glm::dot(ray.direction,L);
    float c = glm::dot(L,L) - radius_ * radius_;
    if (!solveQuadratic(a, b, c, t0, t1)) hit_point.success= false;
    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1; // If t0 is negative, let's use t1 instead.
        if (t0 < 0)  hit_point.success= false; // Both t0 and t1 are negative.
    }

    t = t0;
    hit_point.success= true;

    /*if (!hit_point.success) return hit_point;*/
    glm::vec3 intersection_point = ray.origin + t * ray.direction;
	hit_point.intersection_point = intersection_point;
    hit_point.distance=t;



    hit_point.normale = norm(glm::vec3{intersection_point.x - center_.x, intersection_point.y - center_.y, intersection_point.z - center_.z});
	return hit_point;
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

/*bool intersect(const Ray &ray) const
{
        float t0, t1; // Solutions for t if the ray intersects the sphere
#if 0
        // Geometric solution
        glm::vec3 L = center_ - ray.orig;
        float tca = L.dotProduct(ray.dir);
        // if (tca < 0) return false;
        float d2 = L.dotProduct(L) - tca * tca;
        if (d2 > radius * radius) return false;
        float thc = sqrt(radius * radius - d2);
        t0 = tca - thc;
        t1 = tca + thc;
#else
        // Analytic solution
    glm::vec3 L = ray.orig - center;
        float a = ray.dir.dotProduct(ray.dir);
        float b = 2 * ray.dir.dotProduct(L);
        float c = L.dotProduct(L) - radius * radius;
        if (!solveQuadratic(a, b, c, t0, t1)) return false;
#endif
        if (t0 > t1) std::swap(t0, t1);

        if (t0 < 0) {
            t0 = t1; // If t0 is negative, let's use t1 instead.
            if (t0 < 0) return false; // Both t0 and t1 are negative.
        }

        t = t0;

        return true;
}


}*/