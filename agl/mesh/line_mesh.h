// Copyright, 2020, Savvy Sine, Aline Normoyle
#ifndef AGL_MESH_LINE_MESH_H_
#define AGL_MESH_LINE_MESH_H_

#include "agl/mesh.h"

namespace agl {

/**
 * @brief Base class for meshes
 * 
 * @see TriangleMesh 
 * @see PointMesh 
 * @see LineMesh 
 */
class LineMesh : Mesh {
 public:
  virtual ~LineMesh();

  /**
   * @brief Draw this mesh
   *
   * Typically, users do not need to call this function. It is called from 
   * Renderer.
   * @see Renderer::mesh(const Mesh&)
   */ 
  virtual void render() const;
};

}  // namespace agl
#endif  // AGL_MESH_LINE_MESH_H_
