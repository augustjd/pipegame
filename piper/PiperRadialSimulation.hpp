#pragma once

#include "RadialSimulation.hpp"


class PiperRadialSimulation : public RadialSimulation {
public:
  PiperRadialSimulation(float radius, std::shared_ptr<ShaderProgram> ring_shader);

  void before_update();
  void update() override;

  void spawn_ring();

  void set_player(std::unique_ptr<MeshEntity> player);

  RadialMeshEntity& player() const;

private:
  void check_collisions();
  bool check_collision(const RadialMeshEntity& l, const RadialMeshEntity& r);

  int _player_id;
  std::shared_ptr<ShaderProgram> _ring_shader;
  float _last_spawn = -10.0f;
};
