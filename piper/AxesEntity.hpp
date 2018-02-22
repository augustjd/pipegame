#pragma once

#include <vector>

#include "glitter.hpp"

#include "Entity.hpp"
#include "shaders/MvpShader.hpp"


class AxesEntity : public Entity {
public:
  AxesEntity();

  void draw() override;

private:
  GLuint vao;
  GLuint vbo;

  std::vector<Eigen::Vector3f> _positions;
};
