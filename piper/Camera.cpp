#include "Camera.hpp"

#include "glitter.hpp"

Lens::Lens()
  : _projection(Eigen::Matrix4f::Identity())
{
}


const Eigen::Matrix4f Lens::matrix() const {
  return _projection;
}


Camera::Camera()
  : _lens()
{
}


Camera::Camera(const Lens& lens)
  : _lens(lens)
{
}


void Camera::draw(Entity& entity) {
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
