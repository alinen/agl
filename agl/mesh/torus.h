// Copyright 2011, OpenGL 4.0 Shading language cookbook (David Wolf 2011)

#ifndef AGL_MESH_TORUS_H_
#define AGL_MESH_TORUS_H_

#include "agl/triangle_mesh.h"

namespace agl {

class Torus : public TriangleMesh {
 public:
  Torus(GLfloat outerRadius, GLfloat innerRadius,
      GLuint nsides, GLuint nrings);
};

}  // namespace agl
#endif  // AGL_MESH_TORUS_H_
