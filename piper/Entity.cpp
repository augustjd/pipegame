#include "Entity.hpp"

#include <Eigen/Geometry>


Pose::Pose()
  : _position(Eigen::Vector4f::Zero()),
  _orientation(0, 0, 0, 1)
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
  _matrix = (Eigen::Translation3f(_position.topRows<3>()) * _orientation).matrix();
  //_matrix.setIdentity();
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


void Pose::move(const Eigen::Vector4f& translation) {
  _position += translation;
  std::cout << "new position: " << _position.transpose() << std::endl;
  update_matrix();
  std::cout << "new matrix: " << std::endl << matrix() << std::endl << std::endl;
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


void Entity::move(const Eigen::Vector4f& translation) {
  _pose.move(translation);
}
