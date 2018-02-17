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
  Camera(const Pose& pose = Pose(),
         const Lens& lens = Lens());

  void render(Entity& entity);

  const Lens& lens() const;

private:
  Lens _lens;
};
