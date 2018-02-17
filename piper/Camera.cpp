#include "Camera.hpp"

#include "glitter.hpp"


Camera::Camera(const Lens& lens)
  : _lens(lens)
{
}


void Camera::draw(const Mesh& mesh) {
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
}
