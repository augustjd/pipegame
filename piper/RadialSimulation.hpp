#pragma once

#include <unordered_map>

#include "glitter.hpp"
#include "MeshEntity.hpp"
#include "Camera.hpp"


class RadialSimulation;


class RadialMeshEntity: public Entity {
public:
  RadialMeshEntity(std::unique_ptr<MeshEntity> other, std::shared_ptr<RadialSimulation> simulation, int id);

  float theta() const { return _theta; }
  float omega() const { return _omega; }

  float z() const { return _z; }
  float v() const { return _v; }

  float set_theta(float new_theta) { _theta = new_theta; }
  float set_omega(float new_omega) { _omega = new_omega; }

  float set_z(float new_z) { _z = new_z; }
  float set_v(float new_v) { _v = new_v; }

  void update(float dt);
  void draw() override;

  int id() const { return _id; }
private:
  float measure_radial_width(const Mesh& mesh, float world_radius);
  float measure_axial_depth(const Mesh& mesh, float world_radius);

  void update_pose();

  std::unique_ptr<MeshEntity> _deferred;

  float _theta; // angular position
  float _omega; // angular velocity

  float _z; // axial position
  float _v; // axial velocity

  float _radial_width; // in radians
  float _axial_depth; // in meters

  std::weak_ptr<RadialSimulation> _simulation;
  int _id;
};


class RadialSimulation : public std::enable_shared_from_this<RadialSimulation> {
public:
  RadialSimulation(float radius);

  float radius() const;

  void add_entity(std::unique_ptr<MeshEntity> entity);
  void remove_entity(int id);

  void update();
  void draw(Camera& camera);

private:
  std::unordered_map<int, RadialMeshEntity> _entities;
  float _last_frame;
  float _radius;
};
