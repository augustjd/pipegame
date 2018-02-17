#pragma once

#include "Entity.hpp"
#include "meshes/Mesh.hpp"
#include "shaders/Shader.hpp"


class Lens {
public:
  Lens();

  const Eigen::Matrix4f matrix() const;

private:
  Eigen::Matrix4f _projection;
};


class Camera : public Entity {
public:
  Camera();
  Camera(const Lens& lens);

  void draw(Entity& entity);

  const Lens& lens() const;

private:
  Lens _lens;
};
