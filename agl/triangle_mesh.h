// Copyright, 2011, OpenGL 4.0 Shading language cookbook (David Wolf)
#ifndef AGL_TRIANGLE_MESH_H_
#define AGL_TRIANGLE_MESH_H_

#include <vector>
#include "agl/agl.h"

namespace agl {
class TriangleMesh {
 public:
  virtual ~TriangleMesh();
  virtual void render() const;
  GLuint getVao() const { return vao; }

 protected:
  GLuint nVerts;    // Number of vertices
  GLuint vao;       // The Vertex Array Object

  // Vertex buffers
  std::vector<GLuint> buffers;

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
