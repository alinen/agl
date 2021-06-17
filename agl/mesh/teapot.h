// Copyright 2020,  Savvy Sine, Aline Normoyle

#ifndef AGL_MESH_TEAPOT_H_
#define AGL_MESH_TEAPOT_H_

#include "agl/mesh/triangle_mesh.h"
#include <vector>
#include <glm/glm.hpp>

namespace agl {

/**
 * @brief Draw a teapot mesh
 */ 
class Teapot : public TriangleMesh {
 public:
  /**
   * @brief Create a Teapot mesh
   * @param grid Smoothing level for generating the mesh from bezie patches
   * @param lidTranform Change the position of the teapot lid
   * 
   * Use the renderer to draw a teapot with default settings. 
   * Create a new teapot to create a teapot mesh with different settings. 
   * 
   * @see Renderer::teapot()
   */
  Teapot(int grid, const glm::mat4& lidTransform);

 protected:
  void init() override;

 private:
  int _grid;
  glm::mat4 _lidTransform;

  // From: OpenGL 4.0 Shading language cookbook (David Wolf 2011)
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

};

}  // namespace agl
#endif  // AGL_MESH_TEAPOT_H_
