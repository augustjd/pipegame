#include "meshes/Cylinder.hpp"

Cylinder::Cylinder(float radius, float length, size_t radial_sections, size_t length_sections)
  : Mesh(calculate(radius, length, radial_sections, length_sections))
{
}


Mesh Cylinder::calculate(float radius, float length, size_t radial_sections, size_t length_sections) {
}
