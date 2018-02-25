#pragma once

#include "glitter.hpp"

#include "shaders/PointLightingShader.hpp"
#include "meshes/Mesh.hpp"
#include "Entity.hpp"


class MeshEntity : public Entity {
public:
  MeshEntity(std::shared_ptr<Mesh> mesh,
             std::shared_ptr<ShaderProgram> program,
             const Pose& pose = Pose());

  void draw() override;

  static bool DEBUG;
  bool debug = false;

  const Mesh& mesh();
  void set_color(const Eigen::Vector3f& color);

private:
  std::shared_ptr<Mesh> _mesh;
  PointLightingShader::Material _material;
};
