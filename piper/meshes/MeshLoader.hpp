#pragma once

#include "utils/filesystem.hpp"
#include "meshes/Mesh.hpp"


class MeshLoader {
public:
  std::shared_ptr<Mesh> load(const path& path, bool debug = false);
};
