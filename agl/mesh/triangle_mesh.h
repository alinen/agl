// Copyright, 2020, Savvy Sine, Aline Normoyle
#ifndef AGL_MESH_TRIANGLE_MESH_H_
#define AGL_MESH_TRIANGLE_MESH_H_

#include <vector>
#include "agl/mesh.h"

namespace agl {

/**
 * @brief Base class for indexed triangle meshes
 * 
 * Override this class to create your own meshes.
 * @verbinclude undulate.cpp
 */
class TriangleMesh : public Mesh {
 public:
  /**
   * @brief Draw this mesh
   *
   * Typically, users do not need to call this function. It is called from 
   * Renderer.
   * @see Renderer::mesh(const TriangleMesh&)
   */ 
  virtual void render() const;

 protected:
  GLuint _nIndices = 0;    // Number of triangle vertices

  /**
   * @brief Call initBuffers from init() to set the data for this mesh
   *
   * @see init()
   * @see setIsDynamic(bool)
   */
  void initBuffers(
    std::vector<GLuint>* indices,
    std::vector<GLfloat>* points,
    std::vector<GLfloat>* normals,
    std::vector<GLfloat>* texCoords = nullptr,
    std::vector<GLfloat>* tangents = nullptr);
};

}  // namespace agl
#endif  // AGL_MESH_TRIANGLE_MESH_H_
