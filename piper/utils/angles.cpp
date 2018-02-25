#include "utils/angles.hpp"

#include <cmath>


bool angle_is_between(float theta, float center, float width) {
  auto shortest_distance = std::atan2(std::sin(theta-center), std::cos(theta-center));
  return std::abs(shortest_distance) < width / 2.0f;
}
