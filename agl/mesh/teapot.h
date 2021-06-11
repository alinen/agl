// Copyright 2011, OpenGL 4.0 Shading language cookbook (David Wolf 2011)

#ifndef AGL_MESH_TEAPOT_H_
#define AGL_MESH_TEAPOT_H_

#include "agl/triangle_mesh.h"
#include <glm/glm.hpp>
#include <vector>

namespace agl {

class Teapot : public TriangleMesh
{
 private:
  void generatePatches(std::vector<GLfloat> & p,
                       std::vector<GLfloat> & n,
                       std::vector<GLfloat> & tc,
                       std::vector<GLuint> & el, int grid);

  void buildPatchReflect(int patchNum,
                         std::vector<GLfloat> & B, std::vector<GLfloat> & dB,
                         std::vector<GLfloat> & v, std::vector<GLfloat> & n,
                         std::vector<GLfloat> & tc, std::vector<GLuint> & el,
                         int &index, int &elIndex, int &tcIndex, int grid,
                         bool reflectX, bool reflectY);

  void buildPatch(glm::vec3 patch[][4],
                  std::vector<GLfloat> & B, std::vector<GLfloat> & dB,
                  std::vector<GLfloat> & v, std::vector<GLfloat> & n,
                  std::vector<GLfloat> & tc, std::vector<GLuint> & el,
                  int &index, int &elIndex, int &tcIndex, int grid, 
                  glm::mat3 reflect, bool invertNormal);

  void getPatch(int patchNum, glm::vec3 patch[][4], bool reverseV);

  void computeBasisFunctions(std::vector<GLfloat> & B, 
      std::vector<GLfloat>& dB, int grid);

  glm::vec3 evaluate(int gridU, int gridV, std::vector<GLfloat> & B, 
      glm::vec3 patch[][4]);

  glm::vec3 evaluateNormal(int gridU, int gridV, std::vector<GLfloat>& B, 
      std::vector<GLfloat> & dB, glm::vec3 patch[][4]);

  void fitUnitBox(std::vector<GLfloat>& p, std::vector<GLfloat>& n);

  void moveLid(int grid, std::vector<GLfloat>& p, 
      const glm::mat4& lidTransform);

 public:
  Teapot(int grid, const glm::mat4& lidTransform);
};

}  // namespace agl
#endif  // AGL_MESH_TEAPOT_H_
