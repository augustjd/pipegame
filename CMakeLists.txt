cmake_minimum_required(VERSION 3.0)
project(piper)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++14")

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(vendor/glfw)

option(BUILD_TESTING OFF)
add_subdirectory(vendor/eigen/)

add_subdirectory(vendor/googletest)

add_subdirectory(test)

file(GLOB VENDORS_SOURCES vendor/glad/src/glad.c)
file(GLOB PROJECT_SHADERS piper/shaders/*.comp
  piper/shaders/*.frag
  piper/shaders/*.geom
  piper/shaders/*.vert)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
  Readme.md
  .gitattributes
  .gitignore
  .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE)
add_library(${PROJECT_NAME}_lib
  piper/glitter.hpp

  piper/utils/filesystem.hpp
  piper/utils/filesystem.cpp

  piper/shaders/Shader.hpp
  piper/shaders/Shader.cpp

  piper/shaders/PointLightingShader.hpp
  piper/shaders/PointLightingShader.cpp
  piper/shaders/Uniform.hpp
  piper/shaders/Uniform.cpp

  piper/meshes/Mesh.hpp
  piper/meshes/Mesh.cpp
  piper/meshes/MeshLoader.hpp
  piper/meshes/MeshLoader.cpp
  piper/meshes/Triangle.hpp
  piper/meshes/Triangle.cpp

  piper/Entity.hpp
  piper/Entity.cpp
  piper/MeshEntity.hpp
  piper/MeshEntity.cpp

  piper/Camera.hpp
  piper/Camera.cpp

  ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
  ${VENDORS_SOURCES})

add_executable(${PROJECT_NAME}
  piper/main.cpp
  )

target_link_libraries(${PROJECT_NAME}_lib PUBLIC glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} eigen)

target_include_directories(${PROJECT_NAME}_lib PUBLIC
  piper/
  vendor/glad/include/
  vendor/glfw/include/
  vendor/glm/
  vendor/stb/)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)
