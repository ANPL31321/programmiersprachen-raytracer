#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include "sphere.hpp"
#include "shape.hpp"
#include "box.hpp"
#include <glm/vec3.hpp>
#include <glm/gtx/intersect.hpp>
#include "ray.hpp"
#include "hit_point.hpp"

TEST_CASE(" sphere and box methods ", "[sphere_box_methods]") {
	Sphere s{ "sphere", {0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, 12.34f };
	REQUIRE(1913.55123f == Approx(s.area()));
	REQUIRE(7871.074 == Approx(s.volume()));

	Box b{ "box", {0.0f, 0.0f, 0.0f}, glm::vec3{1.2f, 3.4f, 5.6f}, glm::vec3{7.8f, 9.0f, 10.2f} };
	REQUIRE(186.16f == Approx(b.area()));
	REQUIRE(170.016f == Approx(b.volume()));

	std::cout << s << std::endl;
	std::cout << b << std::endl;
}

TEST_CASE(" intersect_ray_sphere ", "[intersect]")
{
	// Ray
	glm::vec3 ray_origin{ 0.0f, 0.0f, 0.0f };
	// ray direction has to be normalized !
	// you can use :
	// v = glm :: normalize ( some_vector )
	glm::vec3 ray_direction{ 0.0f, 0.0f, 1.0f };
	// Sphere
	glm::vec3 sphere_center{ 0.0f ,0.0f, 5.0f };
	float sphere_radius{ 1.0f };
	float distance = 0.0f;
	auto result = glm::intersectRaySphere(
		ray_origin, ray_direction,
		sphere_center,
		sphere_radius * sphere_radius, // squared radius !!!
		distance);
	REQUIRE(distance == Approx(4.0f));
}

TEST_CASE(" intersect_method_sphere ", "[intersect_method_sphere]") {
	SECTION(" there is intersection ") {
		Ray ray{ glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f} };
		Sphere s{ "sphere1", {0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 5.0f}, 1.0f };
		HitPoint hit_point = s.intersect(ray);
		REQUIRE(true == hit_point.success);
		REQUIRE(4.0f == Approx(hit_point.distance));
		REQUIRE("sphere1" == hit_point.name_intersected_obj);
		REQUIRE(0.0f == Approx(hit_point.color_intersected_onj.r));
		REQUIRE(0.0f == Approx(hit_point.color_intersected_onj.g));
		REQUIRE(0.0f == Approx(hit_point.color_intersected_onj.b));
		REQUIRE(0.0f == Approx(hit_point.intersection_point.x));
		REQUIRE(0.0f == Approx(hit_point.intersection_point.y));
		REQUIRE(4.0f == Approx(hit_point.intersection_point.z));
		REQUIRE(0.0f == Approx(hit_point.ray_direction.x));
		REQUIRE(0.0f == Approx(hit_point.ray_direction.y));
		REQUIRE(1.0f == Approx(hit_point.ray_direction.z));
	}

	SECTION(" there is no intersection ") {
		Ray ray{ glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f} };
		Sphere s{ "sphere2", {0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 5.0f}, 1.0f };
		HitPoint hit_point = s.intersect(ray);
		REQUIRE(false == hit_point.success);
	}

	SECTION(" ray touches sphere ") {
		Ray ray{ glm::vec3{1.0f, 2.0f, 3.0f}, glm::vec3{0.8f, 0.6f, 0.0f} };
		Sphere s{ "sphere3", {1.0f, 1.0f, 1.0f}, glm::vec3{6.0f, 2.0f, 3.0f}, 3.0f };
		HitPoint hit_point = s.intersect(ray);
		REQUIRE(true == hit_point.success);
		REQUIRE(4.0f == Approx(hit_point.distance));
		REQUIRE("sphere3" == hit_point.name_intersected_obj);
		REQUIRE(1.0f == Approx(hit_point.color_intersected_onj.r));
		REQUIRE(1.0f == Approx(hit_point.color_intersected_onj.g));
		REQUIRE(1.0f == Approx(hit_point.color_intersected_onj.b));
		REQUIRE(4.2f == Approx(hit_point.intersection_point.x));
		REQUIRE(4.4f == Approx(hit_point.intersection_point.y));
		REQUIRE(3.0f == Approx(hit_point.intersection_point.z));
		REQUIRE(0.8f == Approx(hit_point.ray_direction.x));
		REQUIRE(0.6f == Approx(hit_point.ray_direction.y));
		REQUIRE(0.0f == Approx(hit_point.ray_direction.z));
	}
}

TEST_CASE(" destructor ", "[destructor]") {
	Color red{ 255 , 0 , 0 };
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	std::cout << std::endl;
	Sphere* s1 = new Sphere{"sphere0", red, position , 1.2f };
	Shape* s2 = new Sphere{"sphere1", red, position , 1.2f };
	s1 -> print(std::cout);
	s2 -> print(std::cout);
	delete s1;
	delete s2;

}

int main(int argc, char *argv[])
{
  return Catch::Session().run(argc, argv);
}
