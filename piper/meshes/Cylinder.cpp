#include "meshes/Cylinder.hpp"

#include <iostream>


Cylinder::Cylinder(float radius, float length, size_t radial_sections, size_t length_sections)
  : Mesh(calculate(radius, length, radial_sections, length_sections))
{
}


Mesh Cylinder::calculate(float radius, float length, size_t radial_sections, size_t length_sections) {
  std::vector<Eigen::Vector3f> _positions;
  std::vector<Eigen::Vector2f> _texture_coords;
  std::vector<Eigen::Vector3f> _normals;
  std::vector<Eigen::Vector3i> _indices;

  for (int j = 0; j < length_sections; ++j) {
    float z_percent = (static_cast<float>(j) / (length_sections - 1));
    float z = length * z_percent;
    for (int i = 0; i < radial_sections; ++i) {
      float x_percent = (static_cast<float>(i) / radial_sections);
      float theta = 2.0f * M_PI * x_percent;
      auto normal = Eigen::Vector3f{std::cos(theta), std::sin(theta), 0};

      Eigen::Vector3f end = normal * radius + (Eigen::Vector3f::UnitZ() * z);
      _positions.emplace_back(end);
      _normals.emplace_back(-normal);
      _texture_coords.emplace_back(Eigen::Vector2f(x_percent, z_percent));
    }
  }

  for (int j = 0; j < length_sections - 1; ++j) {
    auto offset = Eigen::Vector3i::Constant(j * radial_sections);

    for (int i = 0; i < radial_sections; ++i) {
      _indices.emplace_back(offset + Eigen::Vector3i(i, (i+1) % radial_sections, i+radial_sections));
      _indices.emplace_back(offset + Eigen::Vector3i(radial_sections + i, radial_sections + ((i+1) % radial_sections), ((i+1) % radial_sections)));
    }
  }

  return Mesh(std::move(_positions),
              std::move(_texture_coords),
              std::move(_normals),
              std::move(_indices));
}
