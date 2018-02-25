#include "utils/probability.hpp"

#include <iostream>
#include <random>


bool flip_coin(float probability) {
  return random_float(0.0f, 1.0f) < probability;
}


float random_float(float min, float max) {
  static std::default_random_engine generator;
  static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

  return (distribution(generator) * (max - min)) + min;
}
