// Copyright 2020, Savvy Sine, Aline Normoyle

#ifndef AGL_MESH_PYRAMID_H_
#define AGL_MESH_PYRAMID_H_

#include "agl/triangle_mesh.h"

namespace agl {

class Pyramid : public TriangleMesh {
 public:
  Pyramid(float base, float height);
};

}  // namespace agl
#endif  // AGL_MESH_PYRAMID_H_
