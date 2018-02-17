#include "Entity.hpp"

Pose::Pose()
  : _position(Eigen::Vector4f::Zero()),
  _orientation(Eigen::AngleAxisf(0,  Eigen::Vector3f::UnitY()))
{
}


Pose::Pose(const Eigen::Vector4f& position,
           const Eigen::Quaternionf& orientation)
  : _position(position),
  _orientation(orientation)
{
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
