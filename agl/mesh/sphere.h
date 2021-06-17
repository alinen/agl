// Copyright 2020, Savvy Sine, Aline Normoyle

#ifndef AGL_MESH_SPHERE_H_
#define AGL_MESH_SPHERE_H_

#include "agl/mesh/triangle_mesh.h"

namespace agl {

/**
 * @brief Draw a sphere mesh
 */ 
class Sphere : public TriangleMesh {
 public:
  Sphere(float rad, GLuint sl, GLuint st);

 protected:
  void init() override;

 private:
  float _radius;
  GLuint _nSlices;
  GLuint _nStacks;
};

}  // namespace agl
#endif  // AGL_MESH_SPHERE_H_
