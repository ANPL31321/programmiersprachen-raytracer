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

#ifndef RAYTRACER_SCENE_HPP
#define RAYTRACER_SCENE_HPP

class Scene{
public:
    Scene(int x_res, int y_res, std::string const& file_name, Camera const& camera, Color const& ambient);
    // public only for tests
    std::vector<std::shared_ptr<Shape>> shapes_;
    std::map<std::string, std::shared_ptr<Material>> materials_;
private:
    void loadscene();
    void loadmaterial(std::istringstream& line_as_stream);
    void loadshape(std::istringstream& line_as_stream);
    int x_res_, y_res_;
    std::string file_name_;
    //std::vector<std::shared_ptr<Shape>> shapes_;
    //std::map<std::string ,std::shared_ptr<Material>> materials_;
    Camera camera_;
    Color ambient_;
    std::vector<Punktlichquelle> punktlichtquellen_;
};


#endif //RAYTRACER_SCENE_HPP
