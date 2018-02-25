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

  void draw(Camera& camera) override;

  void reset();

  void pause();
  void unpause();
  bool paused() const; 

  void remove_entity(int id) override;

private:
  void check_collisions();
  bool check_collision(const RadialMeshEntity& l, const RadialMeshEntity& r);

  bool _paused = false;
  bool _gameover = false;

  int _player_id;
  std::shared_ptr<ShaderProgram> _ring_shader;
  float _last_spawn = -10.0f;

  int score = 0;
};
