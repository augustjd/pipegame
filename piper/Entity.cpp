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


Pose::Pose(const Eigen::Matrix4f& transform) 
  : _matrix(transform)
{
}


void Pose::update_matrix() {
  std::cout << "Updating matrix with position:" << _position.transpose() << " and orientation:" << _orientation.vec().transpose() << " " << _orientation.w() << std::endl;
  _matrix = (Eigen::Translation3f(_position.topRows<3>()) * _orientation).matrix();
}

template<class T>
Eigen::Matrix<T,4,4> lookAt
(
    Eigen::Matrix<T,3,1> const & eye,
    Eigen::Matrix<T,3,1> const & center,
    Eigen::Matrix<T,3,1> const & up
)
{
    typedef Eigen::Matrix<T,4,4> Matrix4;
    typedef Eigen::Matrix<T,3,1> Vector3;

    Vector3 f = (center - eye).normalized();
    Vector3 u = up.normalized();
    Vector3 s = f.cross(u).normalized();
    u = s.cross(f);

    Matrix4 res;
    res <<  s.x(),s.y(),s.z(),-s.dot(eye),
            u.x(),u.y(),u.z(),-u.dot(eye),
            -f.x(),-f.y(),-f.z(),f.dot(eye),
            0,0,0,1;

    return res;
}


Pose Pose::LookAt(const Eigen::Vector3f& eye,
                  const Eigen::Vector3f& target,
                  const Eigen::Vector3f& up) {
  Eigen::Vector3f z = (eye - target).normalized();
  Eigen::Vector3f x = up.cross(z).normalized();
  Eigen::Vector3f y = x.cross(z).normalized();

  Eigen::Matrix3f rotation;
  rotation.col(0) = x;
  rotation.col(1) = y;
  rotation.col(2) = z;

  Eigen::Quaternionf rotate_z = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitZ(), z);
  Eigen::Quaternionf rotate_x = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitX(), x);

  return Pose(lookAt(eye, target, up));
}


void Pose::move(const Eigen::Vector4f& translation) {
  _position += translation;
  std::cout << "new position: " << _position.transpose() << std::endl;
  update_matrix();
  std::cout << "new matrix: " << std::endl << global_to_local() << std::endl << std::endl;
}


const Eigen::Matrix4f& Pose::global_to_local() const {
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
