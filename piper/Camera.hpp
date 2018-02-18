#pragma once

#include "Entity.hpp"
#include "meshes/Mesh.hpp"
#include "shaders/Shader.hpp"


class Lens {
public:
  Lens();
  Lens(const Eigen::Matrix4f& matrix);

  const Eigen::Matrix4f matrix() const;

  static Lens Orthographic(float left, float right, float top, float bottom, float near, float far);
  static Lens Perspective(float aspect, float fovy_deg, float near, float far);

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
