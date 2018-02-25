#include "PiperRadialSimulation.hpp"

#include <memory>

#include <GLFW/glfw3.h>

#include "meshes/Ring.hpp"

#include "utils/probability.hpp"
#include "utils/angles.hpp"


PiperRadialSimulation::PiperRadialSimulation(float radius, std::shared_ptr<ShaderProgram> ring_shader) 
  : RadialSimulation(radius),
  _ring_shader(ring_shader)
{
  auto program = PointLightingShader::load().value();
}


void PiperRadialSimulation::before_update() {
  if (paused()) return;
  float spawn_rate = 3.5f;
  auto now = glfwGetTime();
  if ((now -_last_spawn) > spawn_rate) {
    _last_spawn = now;
    spawn_ring();
  }
}


void PiperRadialSimulation::draw(Camera& camera) {
  if (_gameover) {
    glClearColor(1, 0, 0, 1);
  } else {
    glClearColor(0, 0, 0, 1);
  }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RadialSimulation::draw(camera);
}


void PiperRadialSimulation::update() {
  if (paused()) return;
  RadialSimulation::update();
  check_collisions();
}


RadialMeshEntity& PiperRadialSimulation::player() const {
  return *_entities.at(_player_id);
}


void PiperRadialSimulation::set_player(std::unique_ptr<MeshEntity> player) {
  auto& entity = add_entity(std::move(player));
  _player_id = entity.id();
  entity.set_theta(3.0f * M_PI / 2.0f);
  entity.set_z(15.0f);
}


void PiperRadialSimulation::spawn_ring() {
  float min_ring = 2 * M_PI * 0.25f;
  float max_ring = 2 * M_PI * 0.80f;

  float ring_arc = random_float(min_ring, max_ring);

  float depth = 3.0f;
  if (flip_coin(0.5f)) {
    depth = 15.0f;
  }
  float radial_thickness = 2.0f;
  
  float tolerance = 0.5f;

  auto id = next_id;
  next_id += 1;
  auto mesh = std::make_unique<MeshEntity>(std::make_shared<Ring>(_radius - radial_thickness - tolerance, _radius - tolerance, depth, ring_arc, 200, 2), _ring_shader);
  auto pair = _entities.emplace(id, std::make_unique<PreCenteredRadialMeshEntity>(std::move(mesh), shared_from_this(), id, ring_arc, depth));

  auto& entity = *pair.first->second;

  entity.set_v(-15.0f);
  entity.set_z(150.0f);


  float min_omega = 2 * M_PI * 0.25f; // 1/8 circle
  float max_omega = 2 * M_PI * 0.60f;  // 3/4 circle;
  entity.set_omega(random_float(min_omega, max_omega) * (flip_coin(0.5f) ? 1 : -1));
}


void PiperRadialSimulation::check_collisions() {
  auto& p = player();
  for (auto& pair : _entities) {
    if (pair.first == p.id()) {
      continue;
    }

    if (check_collision(p, *pair.second)) {
      pair.second->set_color(Eigen::Vector3f::UnitX());
      _gameover = true;
      std::cout << "Game over! Press 'r' to play again." << std::endl;
    }
  }
}


bool PiperRadialSimulation::check_collision(const RadialMeshEntity& l, const RadialMeshEntity& r) {
  float z_front = l.z() + 1.0f;
  return angle_is_between(l.theta(), r.theta(), r.radial_width()) && ((r.z() - r.axial_depth()) <= z_front && r.z() > l.z());
}


void PiperRadialSimulation::reset() {
  auto& p = player();
  p.set_theta(3.0f * M_PI / 2.0f);
  p.set_z(15.0f);

  std::vector<int> to_delete;
  for (auto& entity : _entities) {
    to_delete.emplace_back(entity.first);
  }

  for (auto id : to_delete) {
    if (id != p.id()) {
      remove_entity(id);
    }
  }

  _gameover = false;

  unpause();
}


void PiperRadialSimulation::pause() {
  _paused = true;
  _last_spawn = glfwGetTime() - _last_spawn; // save how long ago the spawn was.
}


void PiperRadialSimulation::unpause() {
  _last_spawn = glfwGetTime() - _last_spawn;
  _last_frame = glfwGetTime();
  _paused = false;
}


bool PiperRadialSimulation::paused() const {
  return _paused || _gameover;
}


void PiperRadialSimulation::remove_entity(int id) {
  RadialSimulation::remove_entity(id);
  score += 1;

  std::cout << "Score: " << score << std::endl;
}
