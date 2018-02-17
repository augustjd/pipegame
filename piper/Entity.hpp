#pragma once

#include <Eigen/Geometry>


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
  Entity();
  Entity(const Pose& pose);

  const Pose& pose() const;

private:
  Pose _pose;
};
