// Copyright 2019, Savvy Sine, Aline Normoyle

#ifndef AGL_MESH_CYLINDER_H_
#define AGL_MESH_CYLINDER_H_

#include "agl/mesh/triangle_mesh.h"

namespace agl {

/**
 * @brief Draw a cylinder mesh
 */ 
class Cylinder : public TriangleMesh {
 public:
  Cylinder(float rad, float len, GLuint sl);
  Cylinder(float rad1, float rad2, float len, GLuint sl);

 protected:
  void init() override;

 private:
  void computeMesh(float rad1, float rad2, float len, GLuint sl);

  float _r1;
  float _r2;
  float _length;
  float _nSlices;
};

}  // namespace agl
#endif  // AGL_MESH_CYLINDER_H_
