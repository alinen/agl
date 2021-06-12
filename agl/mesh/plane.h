// Copyright 2011, OpenGL 4.0 Shading language cookbook (David Wolf 2011)

#ifndef AGL_MESH_PLANE_H_
#define AGL_MESH_PLANE_H_

#include "agl/triangle_mesh.h"

namespace agl {

/**
 * @brief Draw a plane mesh
 */ 
class Plane : public TriangleMesh {
 public:
  Plane(float xsize, float zsize, int xdivs, int zdivs,
      float smax = 1.0f, float tmax = 1.0f);
  void init() override;

 private:
  float _xsize;
  float _zsize;
  int _xdivs;
  int _zdivs;
  float _smax;
  float _tmax;
};

}  // namespace agl
#endif  // AGL_MESH_PLANE_H_
