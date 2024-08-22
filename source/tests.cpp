#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include "sphere.hpp"
#include "shape.hpp"
#include "box.hpp"
#include <glm/vec3.hpp>
#include <glm/gtx/intersect.hpp>
#include "ray.hpp"
#include "hit_point.hpp"
#include "scene.hpp"
#include "set"

TEST_CASE(" sphere and box methods ", "[sphere_box_methods]") {
    Color red{255 , 0 , 0 };
    auto material = std::make_shared<Material>("smth", red, red, red, 0.85f);
	Sphere s{ "sphere", material, glm::vec3{0.0f, 0.0f, 0.0f}, 12.34f };
	REQUIRE(1913.55123f == Approx(s.area()));
	REQUIRE(7871.074 == Approx(s.volume()));

	Box b{ "box",material, glm::vec3{1.2f, 3.4f, 5.6f}, glm::vec3{7.8f, 9.0f, 10.2f} };
	REQUIRE(186.16f == Approx(b.area()));
	REQUIRE(170.016f == Approx(b.volume()));

	std::cout << s << std::endl;
	std::cout << b << std::endl;
}

TEST_CASE(" intersect_ray_sphere ", "[intersect]"){
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
        Color red{255 , 0 , 0 };
        auto material = std::make_shared<Material>("smth", red, red, red, 0.85f);
        SECTION(" there is intersection ") {
		Ray ray{ glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f} };
		Sphere s{ "sphere1", material, glm::vec3{0.0f, 0.0f, 5.0f}, 1.0f };
		HitPoint hit_point = s.intersect(ray);
		REQUIRE(true == hit_point.success);
		REQUIRE(4.0f == Approx(hit_point.distance));
		REQUIRE("sphere1" == hit_point.name_intersected_obj);
		REQUIRE(0.0f == Approx(hit_point.intersection_point.x));
		REQUIRE(0.0f == Approx(hit_point.intersection_point.y));
		REQUIRE(4.0f == Approx(hit_point.intersection_point.z));
		REQUIRE(0.0f == Approx(hit_point.ray_direction.x));
		REQUIRE(0.0f == Approx(hit_point.ray_direction.y));
		REQUIRE(1.0f == Approx(hit_point.ray_direction.z));
	}

	SECTION(" there is no intersection ") {
        Color red{255 , 0 , 0 };
        auto material = std::make_shared<Material>("smth", red, red, red, 0.85f);
		Ray ray{ glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f} };
		Sphere s{ "sphere2", material, glm::vec3{0.0f, 0.0f, 5.0f}, 1.0f };
		HitPoint hit_point = s.intersect(ray);
		REQUIRE(false == hit_point.success);
	}

	SECTION(" ray touches sphere ") {
        Color red{255 , 0 , 0 };
        auto material = std::make_shared<Material>("smth", red, red, red, 0.85f);
		Ray ray{ glm::vec3{1.0f, 2.0f, 3.0f}, glm::vec3{0.8f, 0.6f, 0.0f} };
		Sphere s{ "sphere3", material, glm::vec3{6.0f, 2.0f, 3.0f}, 3.0f };
		HitPoint hit_point = s.intersect(ray);
		REQUIRE(true == hit_point.success);
		REQUIRE(4.0f == Approx(hit_point.distance));
		REQUIRE("sphere3" == hit_point.name_intersected_obj);
		REQUIRE(4.2f == Approx(hit_point.intersection_point.x));
		REQUIRE(4.4f == Approx(hit_point.intersection_point.y));
		REQUIRE(3.0f == Approx(hit_point.intersection_point.z));
		REQUIRE(0.8f == Approx(hit_point.ray_direction.x));
		REQUIRE(0.6f == Approx(hit_point.ray_direction.y));
		REQUIRE(0.0f == Approx(hit_point.ray_direction.z));
	}
}

TEST_CASE(" destructor ", "[destructor]") {
    Color red{255 , 0 , 0 };
    auto material = std::make_shared<Material>("smth", red, red, red, 0.85f);
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	std::cout << std::endl;
	Sphere* s1 = new Sphere{"sphere0", material, position , 1.2f };
	Shape* s2 = new Sphere{"sphere1", material, position , 1.2f };
	s1 -> print(std::cout);
	s2 -> print(std::cout);
	delete s1;
	delete s2;

}

TEST_CASE(" box intersection ", "[box_intersection]") {
    Color red{255 , 0 , 0 };
    auto material = std::make_shared<Material>("smth", red, red, red, 0.85f);
	Box box{ "box", material, { 1.0f, 1.0f, 1.0f }, {3.0f, 4.0f, 5.0f} };
	SECTION("intersection") {
		HitPoint hit_point = box.intersect(norm({ {0.0f, 0.0f, 0.0f}, {1.0f, 2.0f, 3.0f} }));
		REQUIRE(true == hit_point.success);
		REQUIRE(std::sqrt(14) == Approx(hit_point.distance));
		REQUIRE("box" == hit_point.name_intersected_obj);

		REQUIRE(1.0f == Approx(hit_point.intersection_point.x));
		REQUIRE(2.0f == Approx(hit_point.intersection_point.y));
		REQUIRE(3.0f == Approx(hit_point.intersection_point.z));
		REQUIRE(1.0f / std::sqrt(14) == Approx(hit_point.ray_direction.x));
		REQUIRE(2.0f / std::sqrt(14) == Approx(hit_point.ray_direction.y));
		REQUIRE(3.0f / std::sqrt(14) == Approx(hit_point.ray_direction.z));
	}

	SECTION("no intersection") {
		HitPoint hit_point = box.intersect(norm({ {0.0f, 0.0f, 0.0f}, {1.0f, 3.0f, 6.0f} }));
		REQUIRE(false == hit_point.success);
	}

	SECTION("oppesite direction") {
		HitPoint hit_point = box.intersect(norm({ {0.0f, 0.0f, 0.0f}, { -1.0f, -2.0f, -3.0f} }));
		REQUIRE(false == hit_point.success);
	}
}

int load_materials(std::string const& file_path, Scene& scene){

    std::ifstream sdf_file(file_path);
    if (!sdf_file.is_open()) {
        std::cout << "Could not find or open: " << file_path << std::endl;
        return -1;
    }
    std::map<std::string, std::shared_ptr<Material>> map_materials;
    std::string line_buffer;
    while (std::getline(sdf_file, line_buffer)) {
        std::istringstream line_as_stream(line_buffer);
        std::string token;
        line_as_stream >> token;
        if ("define" == token) {
            line_as_stream >> token;
            if ("material" == token) {
                std::shared_ptr<Material> parsed_material = std::make_shared<Material>(Material{"",{0.0f,0.f,0.0f},{0.0f,0.f,0.0f},{0.0f,0.f,0.0f},0.0f});
                line_as_stream >> parsed_material->name_;

                line_as_stream >> parsed_material->ka_.r;
                line_as_stream >> parsed_material->ka_.g;
                line_as_stream >> parsed_material->ka_.b;

                line_as_stream >> parsed_material->kd_.r;
                line_as_stream >> parsed_material->kd_.g;
                line_as_stream >> parsed_material->kd_.b;

                line_as_stream >> parsed_material->ks_.r;
                line_as_stream >> parsed_material->ks_.g;
                line_as_stream >> parsed_material->ks_.b;

                line_as_stream >> parsed_material->m_;

                map_materials.insert(std::make_pair(parsed_material->name_, parsed_material));



                std::cout << "Parsed material " <<
                          parsed_material->name_ << " "
                          << parsed_material->ka_ << " " << parsed_material->kd_ << parsed_material->ks_ << std::endl;
            }
            else {
                std::cout << "Unexpected keyword: " << token << std::endl;
            }
        }
        else {
            std::cout << "Unexpected keyword: " << token << std::endl;
        }

    }

    sdf_file.close();

    return 0;

};
bool operator <( std :: shared_ptr < Material > const & lhs ,
                 std :: shared_ptr < Material > const & rhs)
{ return lhs -> name_ < rhs -> name_ ;};

std::shared_ptr<Material> search_by_name_set(std::string name,std::set<std::shared_ptr<Material>>& set ) {


    auto it = std::find_if(set.begin(), set.end(), [&name](std::shared_ptr<Material> x) { return x->name_ == name; });
    if (it != set.end()) {
        // Element found, 'it' points to the matching element
        return *it;
    } else {
        // Element not found in set
        return nullptr;  // or handle the not found case accordingly
    }
}


auto search_by_name_map(std::string name,std::map<std::string, std::shared_ptr<Material>> map){
    return map.find(name);
}

std::shared_ptr<Material> search_by_name_vector(std::string name,std::vector<std::shared_ptr<Material>>& vector ){
    auto it = std::find_if(vector.begin(), vector.end(), [&name](const std::shared_ptr<Material>& material) {
        return material->name_ == name;
    });

    if (it != vector.end()) {
        return *it;
    } else {
        return nullptr;
    }}



TEST_CASE("searching","[searching]"){
    Scene scene{};
    load_materials("C:/Users/Polina/Desktop/new_folder/programmiersprachen-raytracer/source/materials.sdf",scene);
    std::vector<std::shared_ptr<Material>> material_vector;
    std::set<std::shared_ptr<Material>> material_set;
    std::map<std::string, std::shared_ptr<Material>> material_map;

    for (const auto& pair : scene.materials_) {
        material_vector.push_back(pair.second);
        material_set.insert(pair.second);
        material_map.insert(pair);
    }
    REQUIRE(search_by_name_map("red", material_map)->second == scene.materials_.at("red"));
    REQUIRE(search_by_name_map("yellow", material_map)->second == nullptr);

    REQUIRE(search_by_name_set("red", material_set) == scene.materials_.at("red"));
    REQUIRE(search_by_name_set("yellow", material_set) == nullptr);

    REQUIRE(search_by_name_vector("blue", material_vector) == scene.materials_.at("blue"));
    REQUIRE(search_by_name_vector("green", material_vector) == nullptr);


}

int main(int argc, char* argv[]){
    Scene scene{};
    load_materials("C:/Users/Polina/Desktop/new_folder/programmiersprachen-raytracer/source/materials.sdf",scene);
    return Catch::Session().run(argc, argv);
}





