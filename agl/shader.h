// Copyright 2011, OpenGL 4.0 Shading language cookbook (David Wolf)

#ifndef AGL_SHADER_H_
#define AGL_SHADER_H_

#ifdef WIN32
#pragma warning(disable : 4290)
#endif

#include <string>
#include <map>
#include <stdexcept>
#include "agl/agl.h"
#include "agl/aglm.h"

namespace agl {

class GLSLProgramException : public std::runtime_error {
 public:
  explicit GLSLProgramException(const std::string &msg) :
      std::runtime_error(msg) {}
};

namespace GLSLShader {
  enum Type {
    VERTEX = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
    GEOMETRY = GL_GEOMETRY_SHADER,
    TESS_CONTROL = GL_TESS_CONTROL_SHADER,
    TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
  #ifndef __APPLE__
    COMPUTE = GL_COMPUTE_SHADER
  #endif
  };
}  // namespace GLSLShader

class Shader {
 public:
  Shader();
  ~Shader();

  void compileShader(const std::string& fileName);
  void compileShader(const std::string& fileName, GLSLShader::Type type);
  void compileSource(const std::string &source, GLSLShader::Type type);

  void link();
  void validate();
  void use();
  int getHandle();
  bool isLinked();

  void bindAttribLocation(GLuint location, const char *name);
  void bindFragDataLocation(GLuint location, const char *name);

  void setUniform(const char *name, float x, float y, float z);
  void setUniform(const char *name, const glm::vec2 &v);
  void setUniform(const char *name, const glm::vec3 &v);
  void setUniform(const char *name, const glm::vec4 &v);
  void setUniform(const char *name, const glm::mat4 &m);
  void setUniform(const char *name, const glm::mat3 &m);
  void setUniform(const char *name, const std::vector<glm::mat4> &ms);
  void setUniform(const char *name, float val);
  void setUniform(const char *name, int val);
  void setUniform(const char *name, bool val);
  void setUniform(const char *name, GLuint val);

  void findUniformLocations();

  void printActiveUniforms();
  void printActiveUniformBlocks();
  void printActiveAttribs();

  const char *getTypeString(GLenum type);

 private:
  GLuint handle;
  bool linked;
  std::map<std::string, int> uniformLocations;

  GLint getUniformLocation(const char *name);
  bool fileExists(const std::string &fileName);
  std::string getExtension(const std::string& fileName);

  // Make these private in order to make the object non-copyable
  Shader(const Shader &other) {}
  Shader &operator=(const Shader &other) { return *this; }
};

}  // namespace agl
#endif  // AGL_SHADER_H_
