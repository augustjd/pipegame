#include "meshes/Mesh.hpp"

#include <iostream>


Mesh::Mesh(aligned_vector<Eigen::Vector3f>&& positions,
       aligned_vector<Eigen::Vector2f>&& texture_coordinates,
       aligned_vector<Eigen::Vector3f>&& normals,
       aligned_vector<Eigen::Vector3i>&& indices)
  : _positions(std::move(positions)),
  _texture_coordinates(std::move(texture_coordinates)),
  _normals(std::move(normals)),
  _indices(std::move(indices))
{
  update_geometry();
}


void Mesh::update_geometry() {
  while (_texture_coordinates.size() < _positions.size()) {
    _texture_coordinates.emplace_back(Eigen::Vector2f::Zero());
  }

  while (_normals.size() < _positions.size()) {
    _normals.emplace_back(Eigen::Vector3f::Zero());
  }

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  recreate_buffer(&_position_vbo);
  recreate_buffer(&_texture_coordinate_vbo);
  recreate_buffer(&_normal_vbo);
  recreate_buffer(&_index_vbo);

  glBindBuffer(GL_ARRAY_BUFFER, _position_vbo);
  glBufferData(GL_ARRAY_BUFFER, _positions.size() * 12, _positions.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, _texture_coordinate_vbo);
  glBufferData(GL_ARRAY_BUFFER, _texture_coordinates.size() * 8, _texture_coordinates.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, _normal_vbo);
  glBufferData(GL_ARRAY_BUFFER, _normals.size() * 12, _normals.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_vbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * 12, _indices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}


void Mesh::recreate_buffer(GLuint* buffer) {
  glDeleteBuffers(1, buffer);
  glGenBuffers(1, buffer);
}


const aligned_vector<Eigen::Vector3f>& Mesh::vertices() const {
  return _positions;
}


GLuint Mesh::vertex_array() const {
  return _vao;
}
