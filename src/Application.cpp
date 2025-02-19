// Application.cpp

#include <GL/glew.h>
#include <glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Renderer.h"

// Devuelve struct
struct ShaderProgramSource {
  std::string VertexSource;
  std::string FragmentSource;
};

// Shader parser
static ShaderProgramSource ParseShader(const std::string& filepath) {
  std::ifstream stream(filepath);

  enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;
  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos)
        type = ShaderType::VERTEX;
      else if (line.find("fragment") != std::string::npos)
        type = ShaderType::FRAGMENT;
    } else if (type != ShaderType::NONE) {
      ss[(int)type] << line << '\n';
    }    
  }

  return {ss[0].str(), ss[1].str()};
}

// Compilador de shader
static unsigned int CompileShader(unsigned int type, const std::string& source) {
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  // Manejo de errores
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;
  }
  // Fin del manejo de errores

  return id;
}

// Crea shaders & devuelve funcion id
static unsigned int CreateShader(const std::string& vertexShader,
                                 const std::string& fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

int main(void) {
  GLFWwindow* window;

  // Inicializacion libreria
  if (!glfwInit()) return -1;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* OpenGL context window */
  window = glfwCreateWindow(640, 480, "Hola mundo", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  // Contexto de la ventana
  glfwMakeContextCurrent(window);

  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK) {
    std::cout << "Error" << std::endl;
    return -1;
  }

  std::cout << glGetString(GL_VERSION) << std::endl;

  // Triangle vertices
  float positions[] = { 
    -0.5f, -0.5f, // 0
     0.5f, -0.5f, // 1
     0.5f,  0.5f, // 2
    -0.5f,  0.5f, // 3
  };

  // Vertex indices
  unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
  };

  // Vertex array
  unsigned int vao;
  GLCall(glGenVertexArrays(1, &vao));
  GLCall(glBindVertexArray(vao));

  // Vertex buffer
  unsigned int buffer;
  GLCall(glGenBuffers(1, &buffer));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
  GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

  // Vertex buffer enabler
  GLCall(glEnableVertexAttribArray(0));
  GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

  // Index buffer
  unsigned int ibo;
  GLCall(glGenBuffers(1, &ibo));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

  // Shader parser
  ShaderProgramSource source = ParseShader("src/res/shaders/Basic.shader");
  unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
  GLCall(glUseProgram(shader));

  // Uniform
  int location = glGetUniformLocation(shader, "u_Color");
  ASSERT(location != -1);
  glUniform4f(location, 0.5f, 1.0f, 0.5f, 0.5f);

  // Unbind
  GLCall(glBindVertexArray(0));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

  float r = 0.0f;
  float increment = 0.05f;

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  /* Main loop */
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
  
    // Activamos el shader para que las uniformes se apliquen correctamente.
    GLCall(glUseProgram(shader));  
    GLCall(glUniform4f(location, r, 1.0f, 0.5f, 0.5f));
    
    GLCall(glBindVertexArray(vao));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
  
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
  
    // Lógica para modificar el valor de r
    if (r > 1.0f)
      increment = -0.05f;
    else if (r < 0.0f)
      increment = 0.05f;
    r += increment;
  
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  glDeleteProgram(shader);

  glfwTerminate();
  return 0;
}