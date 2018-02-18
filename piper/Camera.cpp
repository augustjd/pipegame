#include "Camera.hpp"

#include "glitter.hpp"


Lens::Lens()
  : _projection(Eigen::Matrix4f::Identity())
{
}


Lens::Lens(const Eigen::Matrix4f& matrix)
  : _projection(matrix)
{
}


const Eigen::Matrix4f Lens::matrix() const {
  return _projection;
}


Lens Lens::Orthographic(float left, float right, float top, float bottom, float near, float far) {
  Eigen::Matrix4f matrix;

  matrix << 2.0f / (right - left), 0, 0, 0,
            0, 2.0f / (top - bottom), 0, 0, 0,
            0, 0, 2.0f / (far - near), 0,
            -(right+left)/(right-left), -(top+bottom)/(top-bottom), -(far+near)/(far-near);

  return {matrix};
}


Lens Lens::Perspective(float aspect, float near, float far, float fovy) {
  Eigen::Matrix4f matrix;
  float yScale = 1.0f / std::tan(fovy * 0.5F);
  float xScale = yScale / aspect;

  matrix << xScale, 0, 0, 0,
       0, yScale, 0, 0,
       0, 0, -(far+near)/(far-near), -1,
       0, 0, -2*near*far/(far-near), 0;
  return {matrix};
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
