#pragma once

#include "meshes/Mesh.hpp"


class Ring : public Mesh {
public:
  Ring(float inner_radius, float outer_radius, float length, float arc, size_t radial_sections, size_t length_sections);

private:
  static Mesh calculate(float inner_radius, float outer_radius, float length, float arc, size_t radial_sections, size_t length_sections);
};
