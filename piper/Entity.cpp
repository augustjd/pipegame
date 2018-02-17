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
  _matrix = Eigen::Matrix4f::Identity();
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


Entity::Entity(const Pose& pose,
               std::shared_ptr<ShaderProgram> program)
  : _pose(pose),
  _program(program)
{
}


ShaderProgram* Entity::program() {
  return _program.get();
}


const Pose& Entity::pose() const { 
  return _pose; 
}
