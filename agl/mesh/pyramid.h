// Copyright 2020, Savvy Sine, Aline Normoyle

#ifndef AGL_MESH_PYRAMID_H_
#define AGL_MESH_PYRAMID_H_

#include "agl/mesh/triangle_mesh.h"

namespace agl {

/**
 * @brief Draw a pyramid mesh
 */ 
class Pyramid : public TriangleMesh {
 public:
  Pyramid(float base, float height);

 protected:
  void init() override;

 private:
  float _base;
  float _height;
};

}  // namespace agl
#endif  // AGL_MESH_PYRAMID_H_
