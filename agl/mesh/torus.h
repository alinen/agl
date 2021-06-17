// Copyright 2020, Savvy Sine, Aline Normoyle

#ifndef AGL_MESH_TORUS_H_
#define AGL_MESH_TORUS_H_

#include "agl/mesh/triangle_mesh.h"

namespace agl {

/**
 * @brief Draw a torus mesh
 */ 
class Torus : public TriangleMesh {
 public:
  /**
   * @brief Create a torus mesh
   * @param outerRadius The size of the torus (distance from the center)
   * @param innerRadius The size of the hole (distance from the center)
   * @param nsides The number of latitude edges for mesh generation
   * @param nrings The number longitude rings for mesh generation 
   * 
   * Use the renderer to draw a torus with default settings. 
   * Create a new torus to create torus with different settings. 
   * @verbinclude fat-torus.cpp 
   * @see Renderer::torus()
   */
  Torus(GLfloat outerRadius, GLfloat innerRadius,
      GLuint nsides, GLuint nrings);

 protected:
  void init() override;

 private:
  GLfloat _outerRadius;
  GLfloat _innerRadius;
  GLuint _nsides;
  GLuint _nrings;
};

}  // namespace agl
#endif  // AGL_MESH_TORUS_H_
