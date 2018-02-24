#include "glitter.hpp"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.hpp"
#include "meshes/MeshLoader.hpp"
#include "meshes/Triangle.hpp"
#include "meshes/Cylinder.hpp"
#include "meshes/Ring.hpp"
#include "MeshEntity.hpp"
#include "RadialSimulation.hpp"
#include "AxesEntity.hpp"
#include "shaders/PointLightingShader.hpp"
#include "utils/filesystem.hpp"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
      MeshEntity::DEBUG = !MeshEntity::DEBUG; }
}


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

  glEnable(GL_DEPTH_TEST);


  auto model_filepath = path(argc > 1 ? argv[1] : "./models/bunny.obj");

  auto program = PointLightingShader::load().value();
  program.set_material({Eigen::Vector3f(0.2f, 0.5f, 0.5f), 1.0f});

  MeshLoader mesh_loader;
  bool debug_load_model = false;
  auto model_loaded = mesh_loader.load(model_filepath, debug_load_model);

  auto program_shared = std::make_shared<PointLightingShader>(program);
  std::vector<std::shared_ptr<Entity>> entities;

  float world_radius = 20.0f;

  auto simulation = std::make_shared<RadialSimulation>(world_radius);

  entities.emplace_back(std::make_shared<AxesEntity>());

  simulation->add_entity(std::make_unique<MeshEntity>(model_loaded, program_shared));

  entities.emplace_back(std::make_shared<MeshEntity>(std::make_shared<Cylinder>(world_radius, 1000.0f, 30, 20), program_shared));
  entities.emplace_back(std::make_shared<MeshEntity>(std::make_shared<Ring>(2.0f, 3.0f, 5.0f, 1.57f, 30, 8), program_shared));
  if (model_loaded == nullptr) {
    std::cout << "Failed to load " << model_filepath << std::endl;
    return 1;
  }

  auto lens = Lens::Perspective(((float)mWidth) / mHeight, 60.0f, 1e-4, 1000.0f);
  std::cout << "Lens matrix:" << std::endl << lens.matrix() << std::endl << std::endl;
  auto pose = Pose::LookAt(Eigen::Vector3f::Zero(), Eigen::Vector3f::UnitZ());
  Camera camera = {pose, lens};
  std::cout << "Camera view matrix:" << std::endl << camera.pose().global_to_local() << std::endl << std::endl;

  std::cout << "Pose view matrix:" << std::endl << pose.global_to_local() << std::endl << std::endl;

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  program.set_ambient_light(0.3f);

  glfwSetKeyCallback(window, key_callback);


  float speed = 1e-1;
  // Rendering Loop
  while (glfwWindowShouldClose(window) == false) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    } 
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      camera.pose().move(-speed * Eigen::Vector3f::UnitX());
    } 
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      camera.pose().move(speed * Eigen::Vector3f::UnitX());
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.pose().move(speed * Eigen::Vector3f::UnitZ());
      } else {
        camera.pose().move(speed * Eigen::Vector3f::UnitY());
      }
    } 
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.pose().move(-speed * Eigen::Vector3f::UnitZ());
      } else {
        camera.pose().move(-speed * Eigen::Vector3f::UnitY());
      }
    }

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float time = static_cast<float>(glfwGetTime());
    Eigen::Vector3f light_position = {std::cos(time * 2.0f) * 3, 3, std::sin(time * 2.0f) * 3};


    program.set_point_light({ Eigen::Vector3f(1.0f, 1.0f, 1.0f),
                            light_position,
                            0.5f,
                            PointLightingShader::Attenuation{1, 0, 0} });

    for (auto& entity : entities) {
      camera.render(*entity);
    }

    simulation->update();
    simulation->draw(camera);

    // Flip Buffers and Draw
    glfwSwapBuffers(window);
    glfwPollEvents();
  }   

  glfwTerminate();
  return EXIT_SUCCESS;
}
