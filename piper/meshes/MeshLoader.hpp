#pragma once

#include "utils/filesystem.hpp"
#include "meshes/Mesh.hpp"


class MeshLoader {
public:
  std::unique_ptr<Mesh> load(const path& path);
};
