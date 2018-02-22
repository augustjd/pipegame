#include "AxesEntity.hpp"

#include <cassert>


AxesEntity::AxesEntity()
  : Entity(Pose(),
           std::make_shared<MvpShader>(MvpShader::load().value()))
{
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);

  _positions = {
    Eigen::Vector3f::Zero(),
    10 * Eigen::Vector3f::UnitX(),
    Eigen::Vector3f::Zero(),
    10 * Eigen::Vector3f::UnitY(),
    Eigen::Vector3f::Zero(),
    10 * Eigen::Vector3f::UnitZ(),
  };

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, _positions.size() * sizeof(Eigen::Vector3f), _positions.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}


void AxesEntity::draw() {
  auto mvp_program = dynamic_cast<MvpShader*>(program());
  assert(mvp_program);

  mvp_program->set_model(pose());

  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);

  mvp_program->set_color(Eigen::Vector3f::UnitX());
  glDrawArrays(GL_LINES, 0, 2);

  mvp_program->set_color(Eigen::Vector3f::UnitY());
  glDrawArrays(GL_LINES, 2, 2);

  mvp_program->set_color(Eigen::Vector3f::UnitZ());
  glDrawArrays(GL_LINES, 4, 2);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}
