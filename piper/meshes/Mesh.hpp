#pragma once

#include <vector>

#include <Eigen/Core>

#include "glitter.hpp"


class Mesh {
public:
  Mesh(aligned_vector<Eigen::Vector3f>&& positions,
       aligned_vector<Eigen::Vector2f>&& texture_coordinates,
       aligned_vector<Eigen::Vector3f>&& normals,
       aligned_vector<Eigen::Vector3i>&& indices);

  GLuint vertex_array() const;
  const aligned_vector<Eigen::Vector3f>& vertices() const;

private:
  void update_geometry();
  void recreate_buffer(GLuint* buffer);

  GLuint _vao;

  GLuint _position_vbo;
  GLuint _texture_coordinate_vbo;
  GLuint _normal_vbo;
  GLuint _index_vbo;

  aligned_vector<Eigen::Vector3f> _positions;
  aligned_vector<Eigen::Vector2f> _texture_coordinates;
  aligned_vector<Eigen::Vector3f> _normals;
  aligned_vector<Eigen::Vector3i> _indices;
};
