#include "glitter.hpp"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.hpp"
#include "meshes/MeshLoader.hpp"
#include "MeshEntity.hpp"
#include "shaders/Shader.hpp"
#include "utils/filesystem.hpp"


int main() {
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


    auto vertex = *VertexShader::compile(*load_file_to_string("./shaders/vertex.vs"));
    auto fragment = *FragmentShader::compile(*load_file_to_string("./shaders/fragment.fs"));

    auto program = ShaderProgram::link(vertex, fragment).value();

    MeshLoader mesh_loader;
    path cube_filepath = "./models/cube.obj";
    auto cube = mesh_loader.load(cube_filepath);

    MeshEntity entity(cube, std::make_shared<ShaderProgram>(program));

    if (cube == nullptr) {
      std::cout << "Failed to load " << cube_filepath << std::endl;
      return 1;
    }

    Camera camera;

    // Rendering Loop
    while (glfwWindowShouldClose(window) == false) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        camera.draw(entity);

        // Flip Buffers and Draw
        glfwSwapBuffers(window);
        glfwPollEvents();
    }   

    glfwTerminate();
    return EXIT_SUCCESS;
}
