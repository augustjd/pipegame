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


void Camera::draw(ShaderProgram* program, const Mesh& mesh) {
  glUseProgram(*program);
  program->set_camera(*this);

  glBindVertexArray(mesh.vertex_array());
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glDrawElements(GL_TRIANGLES, mesh.vertices().size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
}

const Lens& Camera::lens() const {
  return _lens;
}
