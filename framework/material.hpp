#ifndef RAYTRACER_MATERIAL_HPP
#define RAYTRACER_MATERIAL_HPP
#include <string>
#include "color.hpp"


struct Material{
    Material(std::string const& name,Color const& ka,Color const& kd,Color const& ks, float m);
    Material() = default;
    std::string name_;
    Color ka_,kd_,ks_;
    float m_,t_;
    std::ostream& print(std::ostream& os) const;
};
std::ostream& operator<<(std::ostream& os,Material const& material);


#endif //RAYTRACER_MATERIAL_HPP
