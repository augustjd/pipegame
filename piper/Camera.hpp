#pragma once

#include "Entity.hpp"
#include "meshes/Mesh.hpp"


class Lens {
public:
  Lens();

  Eigen::Matrix4f matrix();

private:
  Eigen::Matrix4f _projection;
};


class Camera : public Entity {
public:
  Camera();
  Camera(const Lens& lens);

  void draw(const Mesh& mesh);

private:
  Lens _lens;
};
