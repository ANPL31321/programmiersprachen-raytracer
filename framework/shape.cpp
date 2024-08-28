#include "shape.hpp"

Shape::Shape(std::string const& name, std::shared_ptr<Material> const& material) :
	name_{ name },
	material_{ material } {
	//std::cout << "Shape Konstruktor Aufruf" << std::endl;
}

std::ostream& Shape::print(std::ostream& os) const {
	return os << "Name: " << name_ << "\n" <<
		"Material: " << (*material_);
}

std::ostream& operator<<(std::ostream& os, Shape const& s) {
	return s.print(os);
}

Shape::~Shape() {
	//std::cout << "Shape Destruktor Aufruf" << std::endl;
}

std::string const& Shape::get_name() const {
	return name_;
}