#pragma once

#include <memory>

#include <Eigen/Geometry>

#include "shaders/Shader.hpp"


class Pose {
public:
  Pose();

  Pose(const Eigen::Vector4f& position,
       const Eigen::Quaternionf& orientation);

  const Eigen::Vector4f& position() const;
  const Eigen::Quaternionf& orientation() const;

  const Eigen::Matrix4f& matrix() const;

  static Pose LookAt(const Eigen::Vector3f& eye,
                     const Eigen::Vector3f& target,
                     const Eigen::Vector3f& up);

private:
  void update_matrix();

  Eigen::Vector4f _position;
  Eigen::Quaternionf _orientation;

  Eigen::Matrix4f _matrix;
};


class Entity {
public:
  Entity(const Pose& pose = Pose(),
         std::shared_ptr<ShaderProgram> program = nullptr);

  virtual ~Entity() = default;

  const Pose& pose() const;

  ShaderProgram* program();

  virtual void draw() {};

private:
  Pose _pose;
  std::shared_ptr<ShaderProgram> _program;
};
