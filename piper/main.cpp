#include "glitter.hpp"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.hpp"
#include "meshes/MeshLoader.hpp"
#include "meshes/Triangle.hpp"
#include "MeshEntity.hpp"
#include "shaders/Shader.hpp"
#include "utils/filesystem.hpp"
int omain(int argc, const char* argv[]);

int main(int argc, const char* argv[]) {
  std::cout << "Testing look at." << std::endl;


  std::cout << "Should be identity:" << std::endl;
  auto pose = Pose::LookAt(Eigen::Vector3f::Zero(),
                           -Eigen::Vector3f::UnitZ(),
                           Eigen::Vector3f::UnitY());
  std::cout << pose.global_to_local() << std::endl;

  std::cout << "Should be identity with a -1 z transform:" << std::endl;
  auto pose2 = Pose::LookAt(Eigen::Vector3f::UnitZ(),
                           Eigen::Vector3f::Zero(),
                           Eigen::Vector3f::UnitY());
  std::cout << pose2.global_to_local() << std::endl;

  std::cout << "Tilted on our side so that X is now the Y direction" << std::endl;
  auto pose3 = Pose::LookAt(Eigen::Vector3f::Zero(),
                            -Eigen::Vector3f::UnitZ(),
                            Eigen::Vector3f::UnitX());
  std::cout << pose3.global_to_local() << std::endl;

  return omain(argc, argv);
}

int omain(int argc, const char* argv[]) {
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

  glEnable(GL_DEPTH_TEST);


  auto model_filepath = path(argc > 1 ? argv[1] : "./models/bunny.obj");

  auto vertex = *VertexShader::compile(*load_file_to_string("./shaders/vertex.vs"));
  auto fragment = *FragmentShader::compile(*load_file_to_string("./shaders/fragment.fs"));

  auto program = ShaderProgram::link(vertex, fragment).value();

  MeshLoader mesh_loader;
  bool debug_load_model = true;
  auto model_loaded = mesh_loader.load(model_filepath, debug_load_model);

  auto program_shared = std::make_shared<ShaderProgram>(program);
  std::vector<std::shared_ptr<Entity>> entities;
  for(int i = 0; i <= 50; ++i) {
    auto zentity = std::make_shared<MeshEntity>(std::make_shared<Triangle>(1.5f, 0.0f), program_shared);
    zentity->move(Eigen::Vector3f::UnitZ() * i);
    entities.emplace_back(zentity);

    auto yentity = std::make_shared<MeshEntity>(std::make_shared<Triangle>(0.5f, 0.0f), program_shared);
    yentity->move(Eigen::Vector3f::UnitY() * i);
    entities.emplace_back(yentity);

    auto xentity = std::make_shared<MeshEntity>(std::make_shared<Triangle>(1.0f, 0.0f), program_shared);
    xentity->move(Eigen::Vector3f::UnitX() * i);
    entities.emplace_back(xentity);
  }

  entities.emplace_back(std::make_shared<MeshEntity>(model_loaded, program_shared));

  if (model_loaded == nullptr) {
    std::cout << "Failed to load " << model_filepath << std::endl;
    return 1;
  }

  auto lens = Lens::Perspective(((float)mWidth) / mHeight, 90, 1e-4, 1000.0f);
  std::cout << "Lens matrix:" << std::endl << lens.matrix() << std::endl << std::endl;
  auto pose = Pose::LookAt(Eigen::Vector3f(8, 8, 8), Eigen::Vector3f::Zero());
  Camera camera = {pose, lens};
  std::cout << "Camera view matrix:" << std::endl << camera.pose().global_to_local() << std::endl << std::endl;

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  float speed = 1e-1;
  // Rendering Loop
  while (glfwWindowShouldClose(window) == false) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    } 
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      camera.move(-speed * Eigen::Vector3f::UnitX());
    } 
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      camera.move(speed * Eigen::Vector3f::UnitX());
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.move(speed * Eigen::Vector3f::UnitZ());
      } else {
        camera.move(speed * Eigen::Vector3f::UnitY());
      }
    } 
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.move(-speed * Eigen::Vector3f::UnitZ());
      } else {
        camera.move(-speed * Eigen::Vector3f::UnitY());
      }
    }

    // Background Fill Color
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& entity : entities) {
      camera.render(*entity);
    }

    // Flip Buffers and Draw
    glfwSwapBuffers(window);
    glfwPollEvents();
  }   

  glfwTerminate();
  return EXIT_SUCCESS;
}
