#include "RadialSimulation.hpp"

#include <cassert>

#include "glitter.hpp"

#include <GLFW/glfw3.h>


RadialMeshEntity::RadialMeshEntity(std::unique_ptr<MeshEntity> other, std::shared_ptr<RadialSimulation> simulation, int id)
  : Entity(other->pose(), other->program()),
  _deferred{std::move(other)},
  _theta{0},
  _omega{0},
  _z{10000.0f},
  _v{0},
  _radial_width{measure_radial_width(_deferred->mesh(), simulation->radius())},
  _axial_depth{measure_axial_depth(_deferred->mesh(), simulation->radius())},
  _simulation{simulation},
  _id{id}
{
  assert(_deferred != 0);
}


RadialMeshEntity::RadialMeshEntity(std::unique_ptr<MeshEntity> other, std::shared_ptr<RadialSimulation> simulation, int id, float radial_width, float axial_depth)
  : Entity(other->pose(), other->program()),
  _deferred{std::move(other)},
  _theta{0},
  _omega{0},
  _z{10000.0f},
  _v{0},
  _radial_width{radial_width},
  _axial_depth{axial_depth},
  _simulation{simulation},
  _id{id}
{
  assert(_deferred != 0);
}


void RadialMeshEntity::set_color(const Eigen::Vector3f& color) {
  _deferred->set_color(color);
}

float RadialMeshEntity::measure_radial_width(const Mesh& mesh, float world_radius) {
  // TODO: fill out with real measurement
  return 0.0f;
}
float RadialMeshEntity::measure_axial_depth(const Mesh& mesh, float world_radius) {
  // TODO: fill out with real measurement
  return 0.0f;
}


void RadialMeshEntity::update(float dt) {
  _theta += _omega * dt;
  _z += _v * dt;

  update_pose();
}


void RadialMeshEntity::update_pose() {
  float world_radius = _simulation.lock()->radius();
  Eigen::Vector3f position = {world_radius * std::cos(_theta), world_radius * std::sin(_theta), _z};
  Eigen::Vector3f up = {std::cos(_theta), std::sin(_theta), 0};

  auto new_pose = Pose::LookAt(position, Eigen::Vector3f::UnitZ() * 10000.0f, -up);
  _deferred->pose() = new_pose;
  pose() = new_pose;
}


void PreCenteredRadialMeshEntity::update_pose() {
  float world_radius = _simulation.lock()->radius();
  Eigen::Vector3f up = {std::cos(_theta), std::sin(_theta), 0};

  auto new_pose = Pose::LookAt(Eigen::Vector3f(0, 0, _z), Eigen::Vector3f::UnitZ() * 10000.0f, -up);
  _deferred->pose() = new_pose;
  pose() = new_pose;
}


void RadialMeshEntity::draw() {
  _deferred->draw();
}


RadialMeshEntity& RadialSimulation::add_entity(std::unique_ptr<MeshEntity> entity) {
  auto id = next_id;
  next_id += 1;
  auto pair = _entities.emplace(id, std::make_unique<RadialMeshEntity>(std::move(entity), shared_from_this(), id));
  return *pair.first->second;
}


RadialMeshEntity& RadialSimulation::add_precentered_entity(std::unique_ptr<MeshEntity> entity) {
  auto id = next_id;
  next_id += 1;
  auto pair = _entities.emplace(id, std::make_unique<PreCenteredRadialMeshEntity>(std::move(entity), shared_from_this(), id));
  return *pair.first->second;
}


void RadialSimulation::remove_entity(int id) {
  _entities.erase(id);
}


void RadialSimulation::update() {
  auto current_frame = glfwGetTime();
  auto elapsed = current_frame - _last_frame;

  std::vector<int> to_delete;
  for (auto& entity : _entities) {
    entity.second->update(elapsed);
    if (entity.second->z() < 0) { // fallen off map.
      to_delete.emplace_back(entity.first);
    }
  }

  _last_frame = current_frame;

  for (auto id : to_delete) {
    remove_entity(id);
  }
}


void RadialSimulation::draw(Camera& camera) {
    for (auto& entity : _entities) {
      camera.render(*entity.second);
    }
}


float RadialSimulation::radius() const {
  return _radius;
}


RadialSimulation::RadialSimulation(float radius) 
  : _radius(radius)
{
}


PreCenteredRadialMeshEntity::PreCenteredRadialMeshEntity(std::unique_ptr<MeshEntity> other, std::shared_ptr<RadialSimulation> simulation, int id)
  : RadialMeshEntity(std::move(other), simulation, id)
{
}


PreCenteredRadialMeshEntity::PreCenteredRadialMeshEntity(std::unique_ptr<MeshEntity> other, std::shared_ptr<RadialSimulation> simulation, int id, float radial_width, float axial_depth)
  : RadialMeshEntity(std::move(other), simulation, id, radial_width, axial_depth)
{
}
