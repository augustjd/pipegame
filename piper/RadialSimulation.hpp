#pragma once

#include <unordered_map>

#include "glitter.hpp"
#include "MeshEntity.hpp"
#include "Camera.hpp"


class RadialSimulation;

class RadialMeshEntity: public Entity {
public:
  RadialMeshEntity(std::unique_ptr<MeshEntity> other, std::shared_ptr<RadialSimulation> simulation, int id);
  RadialMeshEntity(std::unique_ptr<MeshEntity> other, std::shared_ptr<RadialSimulation> simulation, int id, float radial_width, float axial_depth);

  float theta() const { return _theta; }
  float omega() const { return _omega; }

  float z() const { return _z; }
  float v() const { return _v; }

  float set_theta(float new_theta) { _theta = new_theta; }
  float set_omega(float new_omega) { _omega = new_omega; }

  void set_color(const Eigen::Vector3f& color);

  float set_z(float new_z) { _z = new_z; }
  float set_v(float new_v) { _v = new_v; }

  void update(float dt);
  void draw() override;

  int id() const { return _id; }

  float radial_width() const { return _radial_width; }
  float axial_depth() const { return _axial_depth; }
protected:
  float measure_radial_width(const Mesh& mesh, float world_radius);
  float measure_axial_depth(const Mesh& mesh, float world_radius);

  virtual void update_pose();

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


class PreCenteredRadialMeshEntity: public RadialMeshEntity {
public:
  PreCenteredRadialMeshEntity(std::unique_ptr<MeshEntity> other, std::shared_ptr<RadialSimulation> simulation, int id);
  PreCenteredRadialMeshEntity(std::unique_ptr<MeshEntity> other, std::shared_ptr<RadialSimulation> simulation, int id, float radial_width, float axial_depth);

protected:
  void update_pose() override;
};


class RadialSimulation : public std::enable_shared_from_this<RadialSimulation> {
public:
  RadialSimulation(float radius);

  float radius() const;

  RadialMeshEntity& add_entity(std::unique_ptr<MeshEntity> entity);
  RadialMeshEntity& add_precentered_entity(std::unique_ptr<MeshEntity> entity);
  virtual void remove_entity(int id);

  virtual void update();
  virtual void draw(Camera& camera);

protected:
  std::unordered_map<int, std::unique_ptr<RadialMeshEntity>> _entities;
  float _last_frame;
  float _radius;

  int next_id = 0;
};
