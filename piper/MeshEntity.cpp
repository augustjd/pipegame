#include "MeshEntity.hpp"


bool MeshEntity::DEBUG = false;


MeshEntity::MeshEntity(std::shared_ptr<Mesh> mesh,
                       std::shared_ptr<ShaderProgram> program,
                       const Pose& pose)
  : Entity(pose, program),
  _mesh(mesh),
  _material({ Eigen::Vector3f::Random().normalized(), 0.0 })
{
}


void MeshEntity::draw() {
  program()->set_model(pose());

  if (auto point_lighting_shader = dynamic_cast<PointLightingShader*>(program())) {
    point_lighting_shader->set_material(_material);
  }

  glBindVertexArray(_mesh->vertex_array());
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  if (DEBUG) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  glDrawElements(GL_TRIANGLES, _mesh->indices().size() * 3, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
