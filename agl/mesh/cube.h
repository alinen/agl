// Copyright 2020, Savvy Sine, Aline Normoyle

#ifndef AGL_MESH_CUBE_H_
#define AGL_MESH_CUBE_H_

#include "agl/mesh/triangle_mesh.h"

namespace agl {

/**
 * @brief Draw a cube mesh
 */ 
class Cube : public agl::TriangleMesh {
 public:
  explicit Cube(GLfloat size = 1.0f);

 protected:
  void init() override;

 private:
  GLfloat _size;
};

}  // namespace agl
#endif  // AGL_MESH_CUBE_H_
