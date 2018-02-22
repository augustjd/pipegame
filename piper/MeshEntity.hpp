#pragma once

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

private:
  std::shared_ptr<Mesh> _mesh;
  PointLightingShader::Material _material;
};
