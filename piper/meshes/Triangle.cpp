#include "meshes/Triangle.hpp"


Triangle::Triangle(float side, float z)
  : Mesh(
    aligned_vector<Eigen::Vector3f>{side * Eigen::Vector3f(-0.5, -0.5, z), side * Eigen::Vector3f(0, std::sqrt(3)*0.5f, z), side * Eigen::Vector3f(0.5, -0.5, z)},
    aligned_vector<Eigen::Vector2f>{Eigen::Vector2f::Zero(), Eigen::Vector2f::Zero(), Eigen::Vector2f::Zero()},
    aligned_vector<Eigen::Vector3f>{Eigen::Vector3f::UnitZ(),Eigen::Vector3f::UnitZ(),Eigen::Vector3f::UnitZ()},
    aligned_vector<Eigen::Vector3i>{Eigen::Vector3i(0, 1, 2)}
    )
{
}
