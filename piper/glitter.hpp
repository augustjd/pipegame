// Preprocessor Directives
#ifndef GLITTER
#define GLITTER
#pragma once

// System Headers
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Eigen/Core>
#include <unsupported/Eigen/OpenGLSupport>

// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
//     #define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

// Define Some Constants
const int mWidth = 1280;
const int mHeight = 800;

#endif //~ Glitter Header
