// Copyright 2019, Savvy Sine, Aline Normoyle

#ifndef AGL_CYLINDER_H_
#define AGL_CYLINDER_H_

#include "agl/triangle_mesh.h"

namespace agl {

class Cylinder : public TriangleMesh {
 public:
  Cylinder(float rad, float len, GLuint sl);
  Cylinder(float rad1, float rad2, float len, GLuint sl);

 protected:
  void computeMesh(float rad1, float rad2, float len, GLuint sl);
};

}  // namespace agl
#endif  // AGL_CYLINDER_H_
