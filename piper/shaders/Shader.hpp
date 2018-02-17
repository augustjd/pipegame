#pragma once

#include <iostream>

#include "glitter.hpp"

#include "utils/optional.hpp"


class Camera;
class Pose;


class OpenGLResource {
public:
  OpenGLResource(GLint id)
    : _id(id)
  {
  }

  operator GLint() const {
    return _id;
  }

  const GLint _id;
};


class Shader : public OpenGLResource {
protected:
  Shader(GLint id, GLenum type)
    : OpenGLResource(id), _type(type)
  {
  }

  const GLenum _type;

  static nonstd::optional<GLint> compile(const std::string& source, GLenum type);
};


class VertexShader : public Shader {
public:
  static nonstd::optional<VertexShader> compile(const std::string& source);

  VertexShader(GLint id)
    : Shader(id, GL_VERTEX_SHADER)
  {
  }
};


class FragmentShader : public Shader {
public:
  static nonstd::optional<FragmentShader> compile(const std::string& source);

  FragmentShader(GLint id)
    : Shader(id, GL_FRAGMENT_SHADER)
  {
  }
};


class ShaderProgram : public OpenGLResource {
public:
  static nonstd::optional<ShaderProgram> link(const VertexShader& vertex, const FragmentShader& fragment);

  ShaderProgram(GLint id)
    : OpenGLResource(id)
  {
  }

  virtual ~ShaderProgram() = default;

  virtual void set_camera(const Camera& camera);
  virtual void set_model(const Pose& pose);

protected:
  template <typename T>
  bool set_uniform(const std::string& key, const T& value) {
    auto location = glGetUniformLocation(*this, key.c_str());
    if (location == GL_INVALID_VALUE) {
      std::cerr << "Failed to set uniform \"" << key << "\"" << std::endl;
      return false;
    }

    glUniform(location, value);
    return true;
  }
};
