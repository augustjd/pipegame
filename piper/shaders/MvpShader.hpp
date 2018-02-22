#pragma once

#include "shaders/Shader.hpp"
#include "shaders/Uniform.hpp"


class MvpShader : public ShaderProgram {
public:
  static nonstd::optional<MvpShader> load();

  void set_camera(const Camera& camera) override;
  void set_model(const Pose& pose) override;

  void set_color(const Eigen::Vector3f& color);

  MvpShader(GLint id);

private:
  Uniform<Eigen::Matrix4f> _model;
  Uniform<Eigen::Matrix4f> _view;
  Uniform<Eigen::Matrix4f> _projection;

  Uniform<Eigen::Vector4f> _color;
};
