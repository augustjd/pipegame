#pragma once

#include "glitter.hpp"

#include "shaders/Shader.hpp"
#include "shaders/Uniform.hpp"


class PointLightingShader : public ShaderProgram {
public:
  static nonstd::optional<PointLightingShader> load();

  PointLightingShader(GLint id);

  virtual ~PointLightingShader() = default;

  virtual void set_camera(const Camera& camera);
  virtual void set_model(const Pose& pose);

  struct Material {
    Eigen::Vector3f color;
    float reflectance;
  };

  struct Attenuation {
      float constant;
      float linear;
      float exponent;
  };

  struct PointLight {
    Eigen::Vector3f color;
    Eigen::Vector3f position;
    float intensity;
    Attenuation attenuation;
  };

  struct DirectionalLight {
    Eigen::Vector3f color;
    Eigen::Vector3f direction;
    float intensity;
  };

  void set_ambient_light(float value);
  void set_material(const Material& material);

  void set_directional_light(const DirectionalLight& directional_light);
  void set_point_light(const PointLight& point_light);

private:
  Uniform<Eigen::Vector3f> _ambient_light;

  Uniform<Eigen::Vector3f> _material_color;
  Uniform<float> _material_reflectance;
  Uniform<bool> _material_use_color;

  Uniform<Eigen::Vector3f> _directional_light_color;
  Uniform<Eigen::Vector3f> _directional_light_direction;
  Uniform<float> _directional_light_intensity;

  Uniform<Eigen::Vector3f> _point_light_color;
  Uniform<Eigen::Vector3f> _point_light_position;
  Uniform<float> _point_light_intensity;
  Uniform<float> _point_light_attenuation_constant;
  Uniform<float> _point_light_attenuation_linear;
  Uniform<float> _point_light_attenuation_exponent;
};
