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

std::ostream& Sphere::print(std::ostream& os) const {
	return Shape::print(os) <<
		"Radius: " << radius_ << "\n" <<
		"Center: " << "(" << center_.x << ", " << center_.y << ", " << center_.z << ")\n";
}

HitPoint Sphere::intersect(Ray const& ray) const {
    HitPoint hit_point{ false, 0.0f, Shape::name_, Shape::material_, {0.0f, 0.0f, 0.0f}, ray.direction, {0.0f, 0.0f, 0.0f} };

    float t0, t1;

    // Вектор от центра сферы к началу луча
    glm::vec3 L = ray.origin - center_;

    // Коэффициенты квадратного уравнения
    float a = glm::dot(ray.direction, ray.direction);
    float b = 2 * glm::dot(ray.direction, L);
    float c = glm::dot(L, L) - radius_ * radius_;

    // Решаем квадратное уравнение
    if (!solveQuadratic(a, b, c, t0, t1)) {
        // Если нет пересечений, возвращаем hit_point с success = false
        return hit_point;
    }

    // Если корни поменялись местами, т.е. t0 > t1, меняем их обратно
    if (t0 > t1) std::swap(t0, t1);

    // Проверяем, не находится ли пересечение "за" началом луча
    if (t0 < 0) {
        t0 = t1; // Если t0 отрицательный, используем t1
        if (t0 < 0) {
            // Если t1 тоже отрицательный, пересечения нет
            return hit_point;
        }
    }

    // В точке пересечения t будет равно t0
    float t = t0;
    hit_point.success = true;
    hit_point.distance = t;

    // Вычисляем точку пересечения
    glm::vec3 intersection_point = ray.origin + t * ray.direction;
    hit_point.intersection_point = intersection_point;

    // Вычисляем нормаль в точке пересечения и нормализуем её
    hit_point.normale = glm::normalize(intersection_point - center_);

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