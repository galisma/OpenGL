// Shader.cpp
#include <string>

#include <fstream>
#include <iostream>
#include <sstream>

#include "Renderer.h"
#include "Shader.h"
Shader::Shader(const std::string& filepath)
    : m_FilePath(filepath), m_RendererID(0) {
        ShaderProgramSource source = ParseShader(filepath);
        m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
};

Shader::~Shader() {
  GLCall(glDeleteProgram(m_RendererID));
}

// Shader parser
ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
  std::ifstream stream(filepath);

  if (!stream.is_open()) {
    std::cerr << "Error: No se pudo abrir el archivo del shader: " << filepath << std::endl;
    return {"", ""};
  }

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

  std::cout << "Vertex Shader:\n" << ss[0].str() << std::endl;
  std::cout << "Fragment Shader:\n" << ss[1].str() << std::endl;

  return {ss[0].str(), ss[1].str()};
}

// Compilador de shader
unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
  GLCall(unsigned int id = glCreateShader(type));
  const char* src = source.c_str();
  GLCall(glShaderSource(id, 1, &src, nullptr));
  GLCall(glCompileShader(id));

  // Manejo de errores
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl; std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;
  }
  // Fin del manejo de errores

  return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader,
                                  const std::string& fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  // Verificación de linkeo
  GLint linkStatus;
  glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
  if (linkStatus == GL_FALSE) {
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length);
    glGetProgramInfoLog(program, length, &length, message);
    std::cout << "Error de enlace del programa:\n" << message << std::endl;
    glDeleteProgram(program);
    return 0;
  }
  // Fin de verificación

  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Advertencia: uniform '" << name << "' no encontrado." << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}