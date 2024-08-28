//
// Created by Polina on 07.07.2024.
#include "scene.hpp"
#include "vector"
#include <glm/glm.hpp>

#include <iostream>
//file stream for opening files
#include <fstream>
#include <string>
// stringstream for getting parts of the read string (tokens) and
// streaming them into our Material Variables
#include <sstream>
#include "map"
#include <numbers>
#include "sphere.hpp"
#include "glm/gtx/intersect.hpp"
#include <limits>



void Scene::loadmaterial(std::istringstream& line_as_stream) {
    std::shared_ptr<Material> parsed_material = std::make_shared<Material>(Material{ "",{0.0f,0.f,0.0f},{0.0f,0.f,0.0f},{0.0f,0.f,0.0f},0.0f });
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

    materials_.insert(std::make_pair(parsed_material->name_, parsed_material));

    std::cout << "Parsed material " <<
        parsed_material->name_ << " "
        << parsed_material->ka_ << " " << parsed_material->kd_ << parsed_material->ks_ << std::endl;
}

void Scene::loadshape(std::istringstream& line_as_stream) {
    std::string token;
    line_as_stream >> token;
    if ("sphere" == token) {
        std::string name;
        std::string name_of_material;
        glm::vec3 center;
        float radius;

        line_as_stream >> name;
        line_as_stream >> center.x;
        line_as_stream >> center.y;
        line_as_stream >> center.z;
        line_as_stream >> radius;
        line_as_stream >> name_of_material;

        auto material = materials_.find(name_of_material);
        if (material == materials_.end()) {
            std::cout << "Material " << name_of_material << " not found" << std::endl;
            return;
        }
        shapes_.push_back(std::make_shared<Sphere>(Sphere{ name, material->second, center, radius }));
        //shapes_.back()->print(std::cout);
    }
    else if ("box" == token) {
        std::string name;
        std::string name_of_material;
        glm::vec3 min;
        glm::vec3 max;

        line_as_stream >> name;
        line_as_stream >> min.x;
        line_as_stream >> min.y;
        line_as_stream >> min.z;
        line_as_stream >> max.x;
        line_as_stream >> max.y;
        line_as_stream >> max.z;
        line_as_stream >> name_of_material;

        auto material = materials_.find(name_of_material);
        if (material == materials_.end()) {
            std::cout << "Material " << name_of_material << " not found" << std::endl;
            return;
        }

        shapes_.push_back(std::make_shared<Box>(Box{ name, material->second, min, max }));
        shapes_.back()->print(std::cout);
    }
    else {
        std::cout << "Unexpected keyword: " << token << std::endl;
    }
}

void Scene::loadlight(std::istringstream& line_as_stream) {
    Punktlichquelle lightsource{ "", glm::vec3{0.0f, 0.0f, 0.0f}, Color{0.0f, 0.0f, 0.0f} };
    line_as_stream >> lightsource.name;

    line_as_stream >> lightsource.position.x;
    line_as_stream >> lightsource.position.y;
    line_as_stream >> lightsource.position.z;

    line_as_stream >> lightsource.color.r;
    line_as_stream >> lightsource.color.g;
    line_as_stream >> lightsource.color.b;

    line_as_stream >> lightsource.brightness;

    punktlichtquellen_.push_back(std::make_shared<Punktlichquelle>(lightsource));
}

void Scene::loadscene() {
    std::ifstream sdf_file(file_name_);
    if (!sdf_file.is_open()) {
        std::cout << "Could not find or open: " << file_name_ << std::endl;
        return;
    }
    std::string line_buffer;
    while (std::getline(sdf_file, line_buffer)) {
        std::istringstream line_as_stream(line_buffer);
        std::string token;
        line_as_stream >> token;
        if ("define" == token) {
            line_as_stream >> token;
            if ("material" == token) {
                loadmaterial(line_as_stream);
            }
            else if ("shape" == token) {
                loadshape(line_as_stream);
            }
            else if ("ambient" == token) {
                line_as_stream >> ambient_.r;
                line_as_stream >> ambient_.g;
                line_as_stream >> ambient_.b;
            }
            else if ("light" == token) {
                loadlight(line_as_stream);
            }
            else if ("camera" == token) {
                line_as_stream >> camera_.name;
                line_as_stream >> camera_.fov_x;
            }
            else {
                std::cout << "Unexpected keyword: " << token << std::endl;
            }
        }
        else if ("render" == token) {
            line_as_stream >> token;
            line_as_stream >> output_file_;
            line_as_stream >> x_res_;
            line_as_stream >> y_res_;
        }
        else {
            std::cout << "Unexpected keyword: " << token << std::endl;
        }

    }

    for (auto shape : shapes_) {

       // shape->print(std::cout);
    }

    sdf_file.close();
}

Scene::Scene(std::string const& file_name) :
    file_name_{ file_name } {
    loadscene();
    distance_to_screen_ = -(x_res_ / 2.0f) / tan((camera_.fov_x / 2.0f) * std::numbers::pi / 180);
}

Pixel const& Scene::render_pixel(unsigned int x, unsigned int y) const {
    Ray ray = norm(Ray{ camera_.position,
                        glm::vec3{(float)x - x_res_ / 2.0f,
                                  (float)y - y_res_ / 2.0f,
                                  distance_to_screen_} - camera_.position });

    Pixel p{x, y};

    // Инициализируем переменные для хранения ближайшего пересечения
    HitPoint closest_hit;
    closest_hit.distance = std::numeric_limits<float>::max();

    bool hit_found = false;
    for (auto shape : shapes_) {
        HitPoint hit_point = shape->intersect(ray);

        if (hit_point.success) {

            if (hit_point.distance < closest_hit.distance) {
                closest_hit = hit_point;
                hit_found = true;
            }
        }
    }

    if (hit_found) {
        p.color = compute_secondary_rays(closest_hit);
    } else {
    }

    return p;
}



Color Scene::compute_secondary_rays(HitPoint const &hit_point) const {
    Color final_intensity{0.0f, 0.0f, 0.0f};

    for (auto light_source: punktlichtquellen_) {
        glm::vec3 light_dir = norm(light_source->position - hit_point.intersection_point);
        Ray shadow_ray = Ray{hit_point.intersection_point + 0.001f * light_dir, light_dir};
        Color intensity{ 0.0f, 0.0f, 0.0f };

        bool in_shadow=false;

        for (auto shape:shapes_) {
            HitPoint shadow_hit = shape->intersect(shadow_ray);
            if (shadow_hit.success &&
                shadow_hit.distance < glm::length(light_source->position - hit_point.intersection_point)) {
                in_shadow = true;
                break;
            }
        }

        if (!in_shadow) {
            float scalar_product = glm::dot(hit_point.normale, light_dir);
                if (scalar_product>0) {
                    intensity.r = light_source->brightness * hit_point.material_intersected_->kd_.r * scalar_product;
                    intensity.g = light_source->brightness * hit_point.material_intersected_->kd_.g * scalar_product;
                    intensity.b = light_source->brightness * hit_point.material_intersected_->kd_.b * scalar_product;
                }
                final_intensity+=intensity;
            }
        }
    return final_intensity+ambient_;
}




unsigned int Scene::get_x_res() {
    return x_res_;
}

unsigned int Scene::get_y_res() {
    return y_res_;
}

std::string const& Scene::get_output_file() {
    return output_file_;
}