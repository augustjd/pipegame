#pragma once

#include "meshes/Mesh.hpp"


class Cylinder : public Mesh {
public:
  Cylinder(float radius, float length, size_t radial_sections, size_t length_sections);

private:
  static Mesh calculate(float radius, float length, size_t radial_sections, size_t length_sections);
};
