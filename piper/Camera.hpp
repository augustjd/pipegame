#pragma once

#include "Entity.hpp"


class Lens {
public:

private:
  Eigen::Matrix4f _projection;
};


class Camera : public Entity {
public:
  Camera(const Lens& lens);

private:
  Lens _lens;
};
