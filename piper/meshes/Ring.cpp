#include "meshes/Ring.hpp"

#include <iostream>


Ring::Ring(float inner_radius, float outer_radius, float length, float arc, size_t radial_sections, size_t length_sections)
  : Mesh(calculate(inner_radius, outer_radius, length, arc, radial_sections, length_sections))
{
}


Mesh Ring::calculate(float inner_radius, float outer_radius, float length, float arc, size_t radial_sections, size_t length_sections) {
  std::vector<Eigen::Vector3f> _positions;
  std::vector<Eigen::Vector2f> _texture_coords;
  std::vector<Eigen::Vector3f> _normals;
  std::vector<Eigen::Vector3i> _indices;

  // inner cylinder
  for (int j = 0; j < length_sections; ++j) {
    float z_percent = (static_cast<float>(j) / (length_sections - 1));
    float z = length * z_percent;

    for (int i = 0; i < radial_sections; ++i) {
      float x_percent = (static_cast<float>(i) / radial_sections);
      float theta = arc * x_percent;
      auto normal = Eigen::Vector3f{std::cos(theta), std::sin(theta), 0};

      Eigen::Vector3f end = normal * inner_radius + (Eigen::Vector3f::UnitZ() * z);
      _positions.emplace_back(end);
      _normals.emplace_back(-normal); // inward facing normal for inner cylinder
      _texture_coords.emplace_back(Eigen::Vector2f(x_percent, z_percent));
    }
  }

  // outer cylinder
  for (int j = 0; j < length_sections; ++j) {
    float z_percent = (static_cast<float>(j) / (length_sections - 1));
    float z = length * z_percent;

    for (int i = 0; i < radial_sections; ++i) {
      float x_percent = (static_cast<float>(i) / radial_sections);
      float theta = arc * x_percent;
      auto normal = Eigen::Vector3f{std::cos(theta), std::sin(theta), 0};

      Eigen::Vector3f end = normal * outer_radius + (Eigen::Vector3f::UnitZ() * z);
      _positions.emplace_back(end);
      _normals.emplace_back(normal); // outward facing normal for outer cylinder
      _texture_coords.emplace_back(Eigen::Vector2f(x_percent, z_percent));
    }
  }

  // cylinder joins
  for (int j = 0; j < length_sections - 1; ++j) {
    auto inside_offset = Eigen::Vector3i::Constant(j * radial_sections);
    auto outside_offset = Eigen::Vector3i::Constant((j + length_sections) * radial_sections);

    for (int i = 0; i < radial_sections - 1; ++i) {
      _indices.emplace_back(inside_offset + Eigen::Vector3i(i, (i+1) % radial_sections, i+radial_sections));
      _indices.emplace_back(inside_offset + Eigen::Vector3i(radial_sections + i, radial_sections + ((i+1) % radial_sections), ((i+1) % radial_sections)));
      _indices.emplace_back(outside_offset + Eigen::Vector3i(i, (i+1) % radial_sections, i+radial_sections));
      _indices.emplace_back(outside_offset + Eigen::Vector3i(radial_sections + i, radial_sections + ((i+1) % radial_sections), ((i+1) % radial_sections)));
    }
  }

  // flat faces
  size_t outside_to_inside_offset = radial_sections * length_sections;
  for (int j = 0; j < length_sections - 1; ++j) {
    auto start_offset = Eigen::Vector3i::Constant(j * radial_sections);
    Eigen::Vector3i this_ring_triangle = start_offset + Eigen::Vector3i(0, outside_to_inside_offset, radial_sections);
    _indices.emplace_back(this_ring_triangle);
    Eigen::Vector3i next_ring_triangle = start_offset + Eigen::Vector3i(outside_to_inside_offset + radial_sections, outside_to_inside_offset, radial_sections);
    _indices.emplace_back(next_ring_triangle);

    // other face is same pattern, but starting at index r rather than index 0 on each ring
    auto end_cap_offset = Eigen::Vector3i::Constant(radial_sections - 1);
    _indices.emplace_back(this_ring_triangle + end_cap_offset);
    _indices.emplace_back(next_ring_triangle + end_cap_offset);
  }

  // curved caps
  for (int i = 0; i < radial_sections - 1; ++i) {
    // front
    Eigen::Vector3i triangle = Eigen::Vector3i(i, i + outside_to_inside_offset, i+1);
    _indices.emplace_back(triangle);
    Eigen::Vector3i triangle2 = Eigen::Vector3i(i+outside_to_inside_offset, i+1, i+outside_to_inside_offset+1);
    _indices.emplace_back(triangle2);

    // back
    int last_ring_offset = (length_sections - 1) * radial_sections;
    _indices.emplace_back(triangle + Eigen::Vector3i::Constant(last_ring_offset));
    _indices.emplace_back(triangle2 + Eigen::Vector3i::Constant(last_ring_offset));
  }

  return Mesh(std::move(_positions),
              std::move(_texture_coords),
              std::move(_normals),
              std::move(_indices));
}
