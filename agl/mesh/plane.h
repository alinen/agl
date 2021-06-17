// Copyright 2020, Savvy Sine, Aline Normoyle

#ifndef AGL_MESH_PLANE_H_
#define AGL_MESH_PLANE_H_

#include "agl/mesh/triangle_mesh.h"

namespace agl {

/**
 * @brief Draw a plane mesh
 */ 
class Plane : public TriangleMesh {
 public:
  Plane(float xsize, float zsize, int xdivs, int zdivs,
      float smax = 1.0f, float tmax = 1.0f);

 protected:
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
