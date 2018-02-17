#include "meshes/MeshLoader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>


// adapted from https://stackoverflow.com/a/37454181
std::vector<std::string> split(const std::string& str, const std::string& delim, bool allow_empty = false) {
  std::vector<std::string> tokens;
  size_t prev = 0, pos = 0;
  do {
    pos = str.find(delim, prev);
    if (pos == std::string::npos) pos = str.length();
    std::string token = str.substr(prev, pos-prev);
    if (!token.empty() || allow_empty) tokens.push_back(token);
    prev = pos + delim.length();
  } while (pos < str.length() && prev < str.length());
  return tokens;
}


Eigen::Vector2f vector2f_from_line(const std::vector<std::string>& splits) {
  return {
    std::stof(splits.at(1)),
    std::stof(splits.at(2)),
  };
}


Eigen::Vector3f vector3f_from_line(const std::vector<std::string>& splits) {
  return {
    std::stof(splits.at(1)),
    std::stof(splits.at(2)),
    std::stof(splits.at(3)),
  };
}


std::shared_ptr<Mesh> MeshLoader::load(const path& path, bool debug) {
  aligned_vector<Eigen::Vector3f> positions;
  aligned_vector<Eigen::Vector2f> texture_coordinates;
  aligned_vector<Eigen::Vector3f> normals;
  aligned_vector<Eigen::Vector3i> indices;

  std::ifstream file(path);

  if (file.bad()) {
    std::cerr << "Failed to open file " << path << std::endl;
    return nullptr;
  }

  try {
    std::string line;
    for(size_t i = 0; std::getline(file, line); ++i) {
      std::vector<std::string> parts = split(line, " ");
      std::istringstream oss(line);
      std::string type;
      oss >> type;

      if (type == "v") {
        positions.emplace_back(vector3f_from_line(parts));
        if (debug) {
          printf("Line %02zu: v %f %f %f\n", i, positions.back().x(), positions.back().y(), positions.back().z());
        }
      } else if (type == "vn") {
        normals.emplace_back(vector3f_from_line(parts));
        if (debug) {
          printf("Line %02zu: vn %f %f %f\n", i, normals.back().x(), normals.back().y(), normals.back().z());
        }
      } else if (type == "vt") {
        texture_coordinates.emplace_back(vector2f_from_line(parts));
        if (debug) {
          printf("Line %02zu: vt %f %f\n", i, texture_coordinates.back().x(), texture_coordinates.back().y());
        }
      } else if (type == "f") {
        Eigen::Vector3i face;
        for (size_t i = 1; i < 4; ++i) {
          std::vector<std::string> vertex_parts = split(parts.at(i), "/", true);
          int index = std::stoi(vertex_parts.front()) - 1; // only vertex matters, .obj is 1-indexed so subtract 1 for OpenGL
          face(i - 1) = index;
        }
        indices.emplace_back(face);
        if (debug) {
          printf("Line %02zu: f %d %d %d\n", i, face.x(), face.y(), face.z());
        }
      } else {
        if (debug) {
          printf("Line %02zu: Skipped '%s'\n", i, line.c_str());
        }
      }
    }
  } catch (...) {
    return nullptr;
  }

  return std::make_shared<Mesh>(std::move(positions),
                                std::move(texture_coordinates),
                                std::move(normals),
                                std::move(indices));
}
