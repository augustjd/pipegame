#include "utils/filesystem.hpp"

#include <fstream>
#include <streambuf>


nonstd::optional<std::string> load_file_to_string(const path& path) {
  std::ifstream file(path);

  std::string result;

  file.seekg(0, std::ios::end);
  result.reserve(file.tellg());
  file.seekg(0, std::ios::beg);

  result.assign((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());

  if (file.bad()) {
    return {};
  }

  return {std::move(result)};
}
