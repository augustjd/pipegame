#include "Camera.hpp"

#include "glitter.hpp"

Lens::Lens()
  : _projection(Eigen::Matrix4f::Identity())
{
}


const Eigen::Matrix4f Lens::matrix() const {
  return _projection;
}


Camera::Camera(const Pose& pose,
               const Lens& lens)
  : Entity(pose),
  _lens(lens)
{
}


void Camera::render(Entity& entity) {
  auto program = entity.program();
  if (!program) {
    return; // cannot be drawn.
  }

  glUseProgram(*program);
  program->set_camera(*this);

  entity.draw();
  glUseProgram(0);
}

const Lens& Camera::lens() const {
  return _lens;
}
