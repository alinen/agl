// Copyright 2011, OpenGL 4.0 Shading language cookbook (David Wolf)

#ifndef AGL_MESH_SPHERE_H_
#define AGL_MESH_SPHERE_H_

#include "agl/triangle_mesh.h"

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
