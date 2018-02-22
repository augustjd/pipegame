#include "shaders/MvpShader.hpp"

#include "Camera.hpp"
#include "utils/filesystem.hpp"


MvpShader::MvpShader(GLint id)
  : ShaderProgram(id),
  _model("modelMatrix", id),
  _view("viewMatrix", id),
  _projection("projectionMatrix", id),
  _color("color", id)
{
}


nonstd::optional<MvpShader> MvpShader::load() {
  auto vertex = *VertexShader::compile(*load_file_to_string("./shaders/debug/mvp_color.vs"));
  auto fragment = *FragmentShader::compile(*load_file_to_string("./shaders/debug/mvp_color.fs"));

  auto program = ShaderProgram::link(vertex, fragment).value();
  return {MvpShader(program.id())};
}


void MvpShader::set_camera(const Camera& camera) {
  _view.set(camera.pose().global_to_local());
  _projection.set(camera.lens().matrix());
}


void MvpShader::set_model(const Pose& pose) {
  _model.set(pose.local_to_global());
}


void MvpShader::set_color(const Eigen::Vector3f& color) {
  _color.set(color.homogeneous());
}
