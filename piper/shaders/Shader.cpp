#include "shaders/Shader.hpp"

#include "Camera.hpp"


nonstd::optional<GLint> Shader::compile(const std::string& src, GLenum type) {
  const char* source = src.c_str();
  auto shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  GLint status = false;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (!status) {
    int length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    std::unique_ptr<char[]> buffer(new char[length]);
    glGetShaderInfoLog(shader, length, nullptr, buffer.get());
    fprintf(stderr, "%s", buffer.get());
    glDeleteShader(shader);
    return {};
  }

  return {shader};
}


nonstd::optional<FragmentShader> FragmentShader::compile(const std::string& source) {
  auto compiled = Shader::compile(source, GL_FRAGMENT_SHADER);

  if (!compiled) {
    return {};
  }

  return {FragmentShader(*compiled)};
}


nonstd::optional<VertexShader> VertexShader::compile(const std::string& source) {
  auto compiled = Shader::compile(source, GL_VERTEX_SHADER);

  if (!compiled) {
    return {};
  }

  return {VertexShader(*compiled)};
}


nonstd::optional<ShaderProgram> ShaderProgram::link(const VertexShader& vertex, const FragmentShader& fragment) {
  auto program = glCreateProgram();

  glAttachShader(program, vertex);
  glAttachShader(program, fragment);

  glLinkProgram(program);

  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if(!status) {
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    std::unique_ptr<char[]> buffer(new char[length]);
    glGetProgramInfoLog(program, length, nullptr, buffer.get());
    fprintf(stderr, "%s", buffer.get());
    glDeleteProgram(program);
    return {};
  }

  return {program};
}


void ShaderProgram::set_camera(const Camera& camera) {
  set_uniform("projectionMatrix", camera.lens().matrix());
  set_uniform("viewMatrix", camera.pose().global_to_local());
}

void ShaderProgram::set_model(const Pose& pose) {
  set_uniform("modelMatrix", pose.local_to_global());
}
