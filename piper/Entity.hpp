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

private:
  Eigen::Vector4f _position;
  Eigen::Quaternionf _orientation;
};


class Entity {
public:
  Entity();
  Entity(const Pose& pose);

  const Pose& pose() const;

private:
  Pose _pose;
};
