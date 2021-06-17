// copyright 2019, savvy_sine, Aline Normoyle

#ifndef AGL_MESH_CAPSULE_H_
#define AGL_MESH_CAPSULE_H_

#include "agl/mesh/triangle_mesh.h"

namespace agl {

/**
 * @brief Draw a capsule mesh
 */ 
class Capsule : public TriangleMesh {
 public:
  Capsule(float rad, float length, GLuint sl, GLuint st);

 protected:
  void init() override;

 private:
  float _radius;
  float _length;
  GLuint _nSlices;
  GLuint _nStacks;
};

}  // namespace agl
#endif  // AGL_MESH_CAPSULE_H_
