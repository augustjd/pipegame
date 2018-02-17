#include "Entity.hpp"

Pose::Pose()
  : _position(Eigen::Vector4f::Zero()),
  _orientation(Eigen::AngleAxisf(0,  Eigen::Vector3f::UnitY()))
{
  update_matrix();
}


Pose::Pose(const Eigen::Vector4f& position,
           const Eigen::Quaternionf& orientation)
  : _position(position),
  _orientation(orientation)
{
  update_matrix();
}


void Pose::update_matrix() {
  // TODO: position and orient matrix.
}


Pose Pose::LookAt(const Eigen::Vector3f& eye,
                  const Eigen::Vector3f& target,
                  const Eigen::Vector3f& up) {
  Eigen::Vector3f z = (target - eye).normalized();
  Eigen::Vector3f x = z.cross(up).normalized();
  Eigen::Vector3f y = z.cross(x).normalized();

  Eigen::Matrix3f rotation;
  rotation.col(0) = x;
  rotation.col(1) = y;
  rotation.col(2) = z;

  return { eye.homogeneous(), Eigen::Quaternionf(rotation) };
}


const Eigen::Matrix4f& Pose::matrix() const {
  return _matrix;
}


Entity::Entity()
  : _pose()
{
}


Entity::Entity(const Pose& pose)
  : _pose(pose)
{
}


const Pose& Entity::pose() const { 
  return _pose; 
}
