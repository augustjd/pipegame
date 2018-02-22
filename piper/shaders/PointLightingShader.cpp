#include "shaders/PointLightingShader.hpp"

#include "Camera.hpp"
#include "utils/filesystem.hpp"


PointLightingShader::PointLightingShader(GLint id)
  : ShaderProgram(id),
  _ambient_light("ambientLight", id),
  _material_color("material.colour", id),
  _material_reflectance("material.reflectance", id),
  _material_use_color("material.useColour", id),
  _directional_light_color("directionalLight.colour", id),
  _directional_light_direction("directionalLight.direction", id),
  _directional_light_intensity("directionalLight.intensity", id),
  _point_light_color("pointLight.colour", id),
  _point_light_position("pointLight.position", id),
  _point_light_intensity("pointLight.intensity", id),
  _point_light_attenuation_constant("pointLight.att.constant", id),
  _point_light_attenuation_linear("pointLight.att.linear", id),
  _point_light_attenuation_exponent("pointLight.att.exponent", id)
{
}


nonstd::optional<PointLightingShader> PointLightingShader::load() {
  auto vertex = *VertexShader::compile(*load_file_to_string("./shaders/vertex.vs"));
  auto fragment = *FragmentShader::compile(*load_file_to_string("./shaders/fragment.fs"));

  auto program = ShaderProgram::link(vertex, fragment).value();
  return {PointLightingShader(program.id())};
}


void PointLightingShader::set_camera(const Camera& camera) {
  set_uniform("projectionMatrix", camera.lens().matrix());
  set_uniform("viewMatrix", camera.pose().global_to_local());
}

void PointLightingShader::set_model(const Pose& pose) {
  set_uniform("modelMatrix", pose.local_to_global());
}

void PointLightingShader::set_ambient_light(float value) {
  if (!_ambient_light.set(Eigen::Vector3f::Constant(value))) {
    std::cerr << "Failed to set ambient light." << std::endl;
  }
}


void PointLightingShader::set_material(const Material& material) {
  if (!_material_color.set(material.color)) {
    std::cerr << "Failed to set material color." << std::endl;
  }
  if (!_material_reflectance.set(material.reflectance)) {
    std::cerr << "Failed to set material color." << std::endl;
  }
  if (!_material_use_color.set(true)) {
    std::cerr << "Failed to set material color." << std::endl;
  }
}


void PointLightingShader::set_directional_light(const DirectionalLight& directional_light) {
  _directional_light_color.set(directional_light.color);
  _directional_light_intensity.set(directional_light.intensity);
  _directional_light_direction.set(directional_light.direction);
}

void PointLightingShader::set_point_light(const PointLight& point_light) {
  _point_light_color.set(point_light.color);
  _point_light_intensity.set(point_light.intensity);
  _point_light_position.set(point_light.position);
  _point_light_attenuation_constant.set(point_light.attenuation.constant);
  _point_light_attenuation_linear.set(point_light.attenuation.linear);
  _point_light_attenuation_exponent.set(point_light.attenuation.exponent);
}
