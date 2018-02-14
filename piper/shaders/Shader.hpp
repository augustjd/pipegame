#pragma once

#include "glitter.hpp"

#include "utils/optional.hpp"


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
};
