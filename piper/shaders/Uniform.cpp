#include "shaders/Uniform.hpp"


template <>
bool Uniform<float>::set(const float& value) {
  glUseProgram(_program);
  glUniform1f(_location, value);
  return glGetError() == GL_NO_ERROR;
}


template <>
bool Uniform<bool>::set(const bool& value) {
  glUseProgram(_program);
  glUniform1i(_location, value ? 1 : 0);
  return glGetError() == GL_NO_ERROR;
}
