#include "Entity.hpp"

#include <Eigen/Geometry>


Pose::Pose()
  : _position(Eigen::Vector3f::Zero()),
  _orientation(0, 0, 0, 1)
{
  update_matrix();
}


Pose::Pose(const Eigen::Vector3f& position,
           const Eigen::Quaternionf& orientation)
  : _position(position),
  _orientation(orientation)
{
  update_matrix();
}


const Eigen::Vector3f& Pose::position() const {
  return _position;
}


const Eigen::Quaternionf& Pose::orientation() const {
  return _orientation;
}


void Pose::update_matrix() {
  _matrix = (_orientation * Eigen::Translation3f(-_position)).matrix();
  _inverse_matrix = _matrix.inverse();
}


Pose Pose::LookAt(const Eigen::Vector3f& eye,
                  const Eigen::Vector3f& target,
                  const Eigen::Vector3f& up) {
  Eigen::Vector3f f = (target - eye).normalized();
  Eigen::Vector3f s = (f.cross(up)).normalized();
  Eigen::Vector3f u = f.cross(s).normalized();

  Eigen::Matrix3f rotation;
  rotation.row(0) = -s;
  rotation.row(1) = u;
  rotation.row(2) = -f;

  Eigen::Quaternionf r;
  r = rotation;

  return {eye, r};
}


void Pose::move(const Eigen::Vector3f& translation) {
  _position += translation;
  update_matrix();
}


const Eigen::Matrix4f& Pose::global_to_local() const {
  return _matrix;
}


const Eigen::Matrix4f& Pose::local_to_global() const {
  return _inverse_matrix;
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


void Entity::move(const Eigen::Vector3f& translation) {
  _pose.move(translation);
}
