#include "MeshEntity.hpp"

MeshEntity::MeshEntity(std::shared_ptr<Mesh> mesh,
                       std::shared_ptr<ShaderProgram> program,
                       const Pose& pose)
  : Entity(pose, program),
  _mesh(mesh)
{
}


void MeshEntity::draw() {
  program()->set_model(pose());

  glBindVertexArray(_mesh->vertex_array());
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glDrawElements(GL_TRIANGLES, _mesh->indices().size() * 3, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
