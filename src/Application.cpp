// Application.cpp
#include <GL/glew.h>
#include <glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Texture.h"

#include "Shader.h"

int main(void) {
  GLFWwindow* window;

  if (!glfwInit()) return -1;
  glViewport(0, 0, 640, 480);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* OpenGL context window */
  window = glfwCreateWindow(640, 480, "Hola mundo", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK) {
    std::cout << "Error" << std::endl;
    return -1;
  }

  std::cout << glGetString(GL_VERSION) << std::endl;
  {

  float positions[] = {
    -0.5f, -0.5f, 0.0f, 0.0f,// 0
     0.5f, -0.5f, 1.0f, 0.0f,// 1
     0.5f,  0.5f, 1.0f, 1.0f,// 2
    -0.5f,  0.5f, 0.0f, 1.0f,// 3
  };

  unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
  };

  VertexArray va;
  VertexBuffer vb(positions, 4 * 4 * sizeof(float));

  VertexBufferLayout layout;
  layout.Push<float>(2);
  layout.Push<float>(2);
  va.AddBuffer(vb, layout);

  IndexBuffer ib(indices, 6);

  Shader shader("./src/res/shaders/Basic.glsl");
  shader.Bind();
  shader.SetUniform4f("u_Color", 0.5f, 1.0f, 0.5f, 0.5f);

  Texture texture("./src/res/textures/pou_sexo.png");
  texture.Bind();
  shader.SetUniform1i("u_Texture", 0);

  va.Unbind();
  shader.Unbind();
  vb.Unbind();
  ib.Unbind();

  Renderer renderer;

  float r = 0.0f;
  float increment = 0.05f;

  /* Main loop */
  while (!glfwWindowShouldClose(window)) {

    // Renderizar aqui
    renderer.Clear();

    shader.Bind();
    shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

    renderer.Draw(va, ib, shader);

    if (r > 1.0f)
      increment = -0.01f;
  
    else if (r < 0.0f)
      increment = 0.01f;

    r += increment;

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  }
  // Borrar aqui
  glfwTerminate();
  return 0;
}