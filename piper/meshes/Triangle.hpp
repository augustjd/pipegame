#pragma once

#include "meshes/Mesh.hpp"


class Triangle : public Mesh {
public:
  Triangle(float side = 1.0f, float z = -3.0f);
};
