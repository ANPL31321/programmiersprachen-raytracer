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
#include <math.h>



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

    line_as_stream >> parsed_material->t_;

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
    // Создаем луч от камеры через пиксель на экране
    Ray ray = norm(Ray{ camera_.position,
                        glm::vec3{(float)x - x_res_ / 2.0f,
                                  (float)y - y_res_ / 2.0f,
                                  distance_to_screen_} - camera_.position });

    Pixel p{x, y};

    // Инициализируем переменные для хранения ближайшего пересечения
    HitPoint closest_hit;
    closest_hit.distance = std::numeric_limits<float>::max();

    // Перебираем все объекты в сцене
    bool hit_found = false;
    for (auto shape : shapes_) {
        HitPoint hit_point = shape->intersect(ray);

        if (hit_point.success) {
            //std::cout << "Intersection found at distance: " << hit_point.distance << std::endl;

            // Если найдено пересечение ближе, обновляем ближайшее пересечение
            if (hit_point.distance < closest_hit.distance) {
                closest_hit = hit_point;
                hit_found = true;
            }
        }
    }

    // Если найдено пересечение, вычисляем цвет пикселя
    if (hit_found) {
        p.color = compute_secondary_rays(closest_hit, 0, true);
    } else {
        //std::cout << "No intersection found for pixel (" << x << ", " << y << ")" << std::endl;
    }

    return p;
}



Color const& Scene::compute_secondary_rays(HitPoint const &hit_point, int depth, bool is_entry) const {
    Color final_intensity{0.0f, 0.0f, 0.0f};

    glm::vec3 reflected_original_ray_direcrion = compute_reflected_vector(-hit_point.ray_direction, hit_point.normale);
    for (auto shape : shapes_) {
        if (hit_point.name_intersected_obj.compare(shape->get_name()) == 0) {
            continue;
        }
        HitPoint reflected_hit = shape->intersect(norm(Ray{ hit_point.intersection_point, reflected_original_ray_direcrion }));
        HitPoint closest_hit;
        closest_hit.distance = std::numeric_limits<float>::max();
        closest_hit.success = false;

        if (reflected_hit.success) {
            //final_intensity += hit_point.material_intersected_->ks_ * compute_secondary_rays(reflected_hit, depth + 1, t_outside);
            closest_hit = reflected_hit;
        }
        if (closest_hit.success && depth < 5) {
            final_intensity += hit_point.material_intersected_->ks_ * compute_secondary_rays(reflected_hit, depth + 1, is_entry);
        }
    }

    if (hit_point.material_intersected_->t_ > 0.0f) {
        //Ray transparent_ray = norm(Ray{hit_point.intersection_point, compute_transparent_vector(hit_point.ray_direction, hit_point.normale, 0.99f, 1.0f) });
        //Ray transparent_ray{ hit_point.intersection_point + 0.001f * hit_point.ray_direction, hit_point.ray_direction };
        //glm::vec3 transparent_vec;
        Ray refracred_ray;
        if (is_entry) {
            //transparent_vec = compute_transparent_vector(hit_point.ray_direction, hit_point.normale, hit_point.material_intersected_->t_, 1.0f);
            refracred_ray = compute_refracted_ray(hit_point, false);
        }
        else {
            //transparent_vec = compute_transparent_vector(hit_point.ray_direction, hit_point.normale, 1 / hit_point.material_intersected_->t_, 1.0f);
            refracred_ray = compute_refracted_ray(hit_point, true);
        }
        //Ray transparent_ray{ hit_point.intersection_point + 0.001f * transparent_vec, transparent_vec };
        //Ray transparent_ray{ hit_point.intersection_point, glm::refract(hit_point.ray_direction, hit_point.normale, 1.0f) };
        /*if (abs(hit_point.ray_direction.x - transparent_ray.direction.x) > 0.0001f ||
            abs(hit_point.ray_direction.y - transparent_ray.direction.y) > 0.0001f ||
            abs(hit_point.ray_direction.z - transparent_ray.direction.z) > 0.0001f) {
            std::cout << "Attention" << std::endl;
        }*/
        
        HitPoint closest_hit;
        closest_hit.distance = std::numeric_limits<float>::max();
        closest_hit.success = false;

        for (auto shape : shapes_) {
            HitPoint refracted_hit = shape->intersect(refracred_ray);
            if (refracted_hit.success) {
                //final_intensity += compute_secondary_rays(transparent_hit, depth + 1, 1.0f);
                //std::cout << hit_point.name_intersected_obj << " " << transparent_hit.name_intersected_obj << std::endl;
                if (refracted_hit.distance < closest_hit.distance) {
                    closest_hit = refracted_hit;
                }
            }
        }

        if (closest_hit.success && depth < 5) {
            if (closest_hit.name_intersected_obj == hit_point.name_intersected_obj) {
                closest_hit.normale = -closest_hit.normale;
                //closest_hit.ray_direction = compute_transparent_vector(closest_hit.ray_direction, closest_hit.normale, t_outside, closest_hit.material_intersected_->t_);
                is_entry = false;
            }
            else {
                is_entry = true;
            }
            final_intensity += compute_secondary_rays(closest_hit, depth + 1, is_entry);
        }
    }
    for (auto light_source : punktlichtquellen_) {
        glm::vec3 light_dir = norm(light_source->position - hit_point.intersection_point);
        Ray shadow_ray = Ray{ hit_point.intersection_point + 0.001f * light_dir, light_dir };
        Color intensity{ 0.0f, 0.0f, 0.0f };

        bool in_shadow = false;

        float shadow_koeff = 1.0f;

        for (auto shape : shapes_) {
            HitPoint shadow_hit = shape->intersect(shadow_ray);
            if (shadow_hit.success &&
                shadow_hit.distance < glm::length(light_source->position - hit_point.intersection_point)) {
                if (shadow_hit.material_intersected_->t_ > 0.0f) {
                    shadow_koeff *= 0.5;
                }
                else {
                    in_shadow = true;
                    break;
                }
            }
        }

        if (!in_shadow) {
            float scalar_product = glm::dot(hit_point.normale, light_dir);
            if (scalar_product > 0) {
                //intensity.r = light_source->brightness * hit_point.material_intersected_->kd_.r * scalar_product;
                //intensity.g = light_source->brightness * hit_point.material_intersected_->kd_.g * scalar_product;
                //intensity.b = light_source->brightness * hit_point.material_intersected_->kd_.b * scalar_product;
                intensity = hit_point.material_intersected_->kd_ * scalar_product * light_source->brightness * light_source->color * shadow_koeff;
            }
            final_intensity += intensity;
            intensity = { 0.0f, 0.0f, 0.0f };
            glm::vec3 v = glm::normalize(hit_point.ray_direction); // not sure if already normalized (check)
            /*
            intensity.r = light_source->brightness * hit_point.material_intersected_->ks_.r *
                std::pow(glm::dot(reflected_ray, v), hit_point.material_intersected_->m_);
            intensity.g = light_source->brightness * hit_point.material_intersected_->ks_.g *
                std::pow(glm::dot(reflected_ray, v), hit_point.material_intersected_->m_);
            intensity.b = light_source->brightness * hit_point.material_intersected_->ks_.b *
                std::pow(glm::dot(reflected_ray, v), hit_point.material_intersected_->m_);
                */

            glm::vec3 reflected_vector = compute_reflected_vector(light_dir, hit_point.normale);

            final_intensity += hit_point.material_intersected_->ks_ *
                std::pow(glm::dot(reflected_vector, v), hit_point.material_intersected_->m_) *
                light_source->brightness * light_source->color;
        }
    }
    return final_intensity + ambient_ * hit_point.material_intersected_->ka_;
}

glm::vec3 const& Scene::compute_transparent_vector(glm::vec3 const& v, glm::vec3 const& normale, float t, float t_outside) const {
    // v, normale are normalized (check if it's true)
    glm::vec3 n;
    if (glm::dot(-v, normale) < 0.0f) {
        n = -normale;
    }
    else {
        n = normale;
    }
    //n = normale;
    /*if (abs(glm::dot(-v, n)) < 0.0001f) {
        return v;
    }*/
    float sin_a = std::pow(1 - std::pow(glm::dot(-v, n), 2), 0.5);
    float sin_b = sin_a * t_outside / t; // длина перпендикуляра
    float cos_b = std::pow(1 - std::pow(sin_b, 2), 0.5); // длина проэкции по нормали
    // (<n, r>  * r) / (|n| * |r|)
    glm::vec3 proj_v_on_n = n * abs(glm::dot(-v, n));
    // porj = - ray + perp -> perp = proj + ray
    glm::vec3 perpendicular = glm::normalize(proj_v_on_n + v);
    
    return -n * cos_b + perpendicular * sin_b;
}

Ray const& Scene::compute_refracted_ray(HitPoint const& hit_point, bool reversed) const {
    // ray_direction and normale are normalized
    // cos_a = <-ray_direction, normale>
    // sin_a = sqrt(1 - (cos_a)^2)
    float sin_a = std::pow(1 - std::pow(glm::dot(-hit_point.ray_direction, hit_point.normale), 2), 0.5);
    // Snell's law: t1 * sin_a = t2 * sin_b, t1 = 1 (air) -> sin_b = sin_a / t2, t2 = t_
    // if reversed: t2 = 1 (air) -> sin_b = sin_a * t1, t1 = t_
    float sin_b; // длина перпендикуляра
    if (!reversed) {
        sin_b = sin_a / hit_point.material_intersected_->t_; 
    }
    else {
        sin_b = sin_a / (1 / hit_point.material_intersected_->t_);
    }
    float cos_b = std::pow(1 - std::pow(sin_b, 2), 0.5); // длина проэкции по нормали
    // proj_r_on_n = (<-ray_direction, normale> * normale) / (|ray_direction| * |normale|),
    // |ray_direction| == |normale| == 1 ->
    // -> proj_r_on_n = <-ray_direction, normale> * normale
    glm::vec3 proj_r_on_n = hit_point.normale * glm::dot(-hit_point.ray_direction, hit_point.normale);
    // porj_r_on_n = - ray + perp -> perp = proj + ray
    glm::vec3 perpendicular = glm::normalize(proj_r_on_n + hit_point.ray_direction);

    glm::vec3 refracted_direction = -hit_point.normale * cos_b + perpendicular * sin_b;
    Ray refracted_ray{ hit_point.intersection_point + 0.001f * refracted_direction, refracted_direction };
    
    return refracted_ray;
}

/*Color Scene::compute_secondary_rays(HitPoint const &hit_point) const {
    // for now ignore shadow and reflection (later probably use reqursion)
    // for now there is only one light source
    Color final_intensity{0.0f, 0.0f, 0.0f};
    for (auto light_source: punktlichtquellen_) {

        Color intensity{ 0.0f, 0.0f, 0.0f };
        Ray ray = norm(Ray{light_source->position,
                           (norm(light_source->position)-norm(hit_point.intersection_point))}); //вычисляем вектор от источника до точки пересечения луча камеру и объекта
        float scalar_product = glm::dot(hit_point.normale, ray.direction);//считаем скалярное произведение между нормалью в этой точке и
        if (scalar_product>0) {
            //intensity.r = light_source->brightness * hit_point.material_intersected_->kd_.r * scalar_product;
            //intensity.g = light_source->brightness * hit_point.material_intersected_->kd_.g * scalar_product;
            //intensity.b = light_source->brightness * hit_point.material_intersected_->kd_.b * scalar_product;
            intensity = hit_point.material_intersected_->kd_ * scalar_product * light_source->brightness;
        }
        final_intensity+=intensity;

        intensity = { 0.0f, 0.0f, 0.0f };
        glm::vec3 v = glm::normalize(hit_point.ray_direction); // not sure if already normalized (check)
        /*
        intensity.r = light_source->brightness * hit_point.material_intersected_->ks_.r *
            std::pow(glm::dot(reflected_ray, v), hit_point.material_intersected_->m_);
        intensity.g = light_source->brightness * hit_point.material_intersected_->ks_.g *
            std::pow(glm::dot(reflected_ray, v), hit_point.material_intersected_->m_);
        intensity.b = light_source->brightness * hit_point.material_intersected_->ks_.b *
            std::pow(glm::dot(reflected_ray, v), hit_point.material_intersected_->m_);
            */

        /*glm::vec3 reflected_vector = compute_reflected_vector(ray.direction, hit_point.normale);

        final_intensity += hit_point.material_intersected_->ks_ *
                           std::pow(glm::dot(reflected_vector, v), hit_point.material_intersected_->m_) *
                           light_source->brightness;
    }
    return final_intensity+ambient_;
}*/


glm::vec3 const& Scene::compute_reflected_vector(glm::vec3 const& v, glm::vec3 const& normale) const {
    // и луч и нормаль заране нормированы
    // ищем проецию луча на нормаль
    // (<n, r>  * r) / (|n| * |r|)
    glm::vec3 proj_v_on_n = normale * glm::dot(v, normale);
    // porj = ray + perp -> perp = proj - ray
    glm::vec3 perpendicular = proj_v_on_n - v;
    // reflected_ray = ray + perp * 2
    perpendicular *= 2;
    return v + perpendicular;
}

/*Color Scene::compute_secondary_rays(HitPoint const& hit_point) const {
    // for now ignore shadow and reflection (later probably use reqursion)
    // for now there is only one light source
    Color final_intensity{ 0.0f, 0.0f, 0.0f };
    Color intensity{ 0.0f, 0.0f, 0.0f };
    for (auto light_source : punktlichtquellen_) {
        Ray ray = norm(Ray{ hit_point.intersection_point, {light_source->position.x - hit_point.intersection_point.x, light_source->position.y - hit_point.intersection_point.y, light_source->position.z - hit_point.intersection_point.z} }); // LoD!!!!
        bool intersection = false;
        for (auto shape : shapes_) {
            HitPoint hit_point = shape->intersect(ray);
            if (hit_point.success) {
                intersection = true;
                //std::cout << hit_point.distance << std::endl;
                break;
            }
        }
        if (intersection) {
            //std::cout << ray.direction.x << std::endl;
        }
        if (!intersection) {
            // LoD!!!
            float scalar_product = hit_point.normale.x * ray.direction.x +
                                   hit_point.normale.y * ray.direction.y + hit_point.normale.z * ray.direction.z;
            intensity.r = light_source->brightness * hit_point.material_intersected_->kd_.r * scalar_product;
            intensity.g = light_source->brightness * hit_point.material_intersected_->kd_.g * scalar_product;
            intensity.b = light_source->brightness * hit_point.material_intersected_->kd_.b * scalar_product;
        }
        final_intensity+=intensity;
    }
    return final_intensity;
}*/




unsigned int Scene::get_x_res() {
    return x_res_;
}

unsigned int Scene::get_y_res() {
    return y_res_;
}

std::string const& Scene::get_output_file() {
    return output_file_;
}