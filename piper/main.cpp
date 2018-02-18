#include "glitter.hpp"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.hpp"
#include "meshes/MeshLoader.hpp"
#include "MeshEntity.hpp"
#include "shaders/Shader.hpp"
#include "utils/filesystem.hpp"


int main(int argc, const char* argv[]) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  auto window = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

  // Check for Valid Context
  if (window == nullptr) {
    fprintf(stderr, "Failed to Create OpenGL Context");
    return EXIT_FAILURE;
  }

  // Create Context and Load OpenGL Functions
  glfwMakeContextCurrent(window);
  gladLoadGL();
  fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));


  auto model_filepath = path(argc > 1 ? argv[1] : "./models/bunny.obj");

  auto vertex = *VertexShader::compile(*load_file_to_string("./shaders/vertex.vs"));
  auto fragment = *FragmentShader::compile(*load_file_to_string("./shaders/fragment.fs"));

  auto program = ShaderProgram::link(vertex, fragment).value();

  MeshLoader mesh_loader;
  bool debug_load_model = false;
  auto model = mesh_loader.load(model_filepath, debug_load_model);

  MeshEntity entity(model, std::make_shared<ShaderProgram>(program));

  if (model == nullptr) {
    std::cout << "Failed to load " << model_filepath << std::endl;
    return 1;
  }

  Camera camera(Pose::LookAt(Eigen::Vector3f(0, 0, 0), -Eigen::Vector3f::UnitZ()));

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  float speed = 1e-2;
  // Rendering Loop
  while (glfwWindowShouldClose(window) == false) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    } 
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      camera.move(speed * Eigen::Vector4f::UnitX());
    } 
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      camera.move(-speed * Eigen::Vector4f::UnitX());
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      camera.move(-speed * Eigen::Vector4f::UnitY());
    } 
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      camera.move(speed * Eigen::Vector4f::UnitY());
    }

    // Background Fill Color
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    camera.render(entity);

    // Flip Buffers and Draw
    glfwSwapBuffers(window);
    glfwPollEvents();
  }   

  glfwTerminate();
  return EXIT_SUCCESS;
}
