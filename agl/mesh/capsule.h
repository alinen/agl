// copyright 2019, savvy_sine, Aline Normoyle

#ifndef AGL_CAPSULE_H_
#define AGL_CAPSULE_H_

#include "agl/triangle_mesh.h"

namespace agl {

class Capsule : public TriangleMesh {
 public:
  Capsule(float rad, float length, GLuint sl, GLuint st);
};

}  // namespace agl
#endif  // AGL_CAPSULE_H_
