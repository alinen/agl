// Copyright 2011, OpenGL 4.0 Shading language cookbook (David Wolf 2011)

#ifndef AGL_MESH_CUBE_H_
#define AGL_MESH_CUBE_H_

#include "agl/triangle_mesh.h"

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
