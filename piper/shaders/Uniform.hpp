#pragma once

#include <iostream>
#include <string>

#include "glitter.hpp"


template <typename T>
class Uniform {
public:
  Uniform(const std::string& name, GLint program)
    : _program(program),
    _name(name),
    _location(glGetUniformLocation(program, name.c_str()))
  {
    if (_location < 0) {
      throw std::logic_error("Failed to get uniform location for '" + name + "'");
    } else {
      std::cout << name << " has got location " << _location << std::endl;
    }
  }

  bool set(const T& value) {
    glUseProgram(_program);
    glUniform(_location, value);
    return glGetError() == GL_NO_ERROR;
  }

  const T& get() {
    return _value;
  }

private:
  GLint _program;
  std::string _name;
  GLint _location;
  T _value;
};


template <>
bool Uniform<float>::set(const float& value);


template <>
bool Uniform<bool>::set(const bool& value);
