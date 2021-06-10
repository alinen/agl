// Copyright 2011, OpenGL 4.0 Shading language cookbook (David Wolf 2011)

#ifndef AGL_CUBE_H_
#define AGL_CUBE_H_

#include "agl/triangle_mesh.h"

namespace agl {

class Cube : public agl::TriangleMesh
{
 public:
   Cube(GLfloat size = 1.0f);
};

}  // namespace agl
#endif  // AGL_CUBE_H_
