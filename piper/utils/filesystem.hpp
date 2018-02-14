#pragma once

#include <string>

#include "utils/optional.hpp"

using path = std::string;

nonstd::optional<std::string> load_file_to_string(const path& path);
