//
// Created by Polina on 07.07.2024.
//

#include <string>
#include "shape.hpp"
#include <vector>
#include <map>
#include "camera.hpp"
#include "color.hpp"
#include "punktlichtquelle.hpp"
#include "sphere.hpp"
#include "box.hpp"
#include "pixel.hpp"

#ifndef RAYTRACER_SCENE_HPP
#define RAYTRACER_SCENE_HPP

class Scene {
public:
    Scene(std::string const& file_name);
    Pixel const& render_pixel(unsigned int x, unsigned int y) const;
    unsigned int get_x_res();
    unsigned int get_y_res();
    std::string const& get_output_file();
private:
    void load_scene();
    void load_material(std::istringstream& line_as_stream);
    void load_shape(std::istringstream& line_as_stream);
    void load_light(std::istringstream& line_as_stream);
    void load_transformations(std::istringstream& line_as_stream);
    Color const& compute_secondary_rays(HitPoint const &hit_point, int depth, bool is_entry) const;
    glm::vec3 const& compute_reflected_vector(glm::vec3 const& v, glm::vec3 const& normale) const;
    Ray const& compute_refracted_ray(HitPoint const& hit_point, bool reversed) const;
    unsigned int x_res_ = 0, y_res_ = 0;
    std::string output_file_ = "";
    std::string file_name_;
    std::vector<std::shared_ptr<Shape>> shapes_;
    std::map<std::string, std::shared_ptr<Material>> materials_;
    Camera camera_{ "", 0.0f };
    Color ambient_{ 0.0f, 0.0f, 0.0f };
    std::vector<std::shared_ptr<Punktlichquelle>> punktlichtquellen_;
    float distance_to_screen_ = 0.0f;
};

#endif //RAYTRACER_SCENE_HPP
