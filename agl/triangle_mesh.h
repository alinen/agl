// Copyright, 2020, Savvy Sine, Aline Normoyle
#ifndef AGL_TRIANGLE_MESH_H_
#define AGL_TRIANGLE_MESH_H_

#include <vector>
#include "agl/agl.h"

namespace agl {

/**
 * @brief Base class for indexed triangle meshes
 * 
 * Override this class to create your own meshes.
 * @verbinclude undulate.cpp
 */
class TriangleMesh {
 public:
  virtual ~TriangleMesh();
  virtual void render() const;
  GLuint getVao() const { return _vao; }

 protected:
  GLuint _nVerts = 0;    // Number of vertices
  GLuint _vao = 0;       // The Vertex Array Object
  bool _initialized = false;

  // Vertex buffers
  std::vector<GLuint> buffers;

  virtual void init() = 0;
  virtual void initBuffers(
    std::vector<GLuint>* indices,
    std::vector<GLfloat>* points,
    std::vector<GLfloat>* normals,
    std::vector<GLfloat>* texCoords = nullptr,
    std::vector<GLfloat>* tangents = nullptr);

  virtual void deleteBuffers();
};

}  // namespace agl
#endif  // AGL_TRIANGLE_MESH_H_
