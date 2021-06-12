// Copyright 2020, Savvy Sine, Aline Normoyle
#include "agl/mesh/pyramid.h"
#include <cstdio>
#include <vector>
#include "agl/aglm.h"

namespace agl {

Pyramid::Pyramid(float size, float len) {
  _base = size;
  _height = len;
}

void Pyramid::init() {
  GLfloat side2 = _base / 2.0f;

  std::vector<GLfloat> p = {
    -side2, -side2, 0.0,  // 0
     side2, -side2, 0.0,  // 1
     side2,  side2, 0.0,  // 2
    -side2,  side2, 0.0,  // 3

       0.0,    0.0, _height,
     side2,  side2, 0.0,  // 2
    -side2,  side2, 0.0,  // 3

       0.0,    0.0, _height,
     side2, -side2, 0.0,  // 1
     side2,  side2, 0.0,  // 2

       0.0,    0.0, _height,
    -side2, -side2, 0.0,  // 0
     side2, -side2, 0.0,  // 1

       0.0,    0.0, _height,
    -side2,  side2, 0.0,  // 3
    -side2, -side2, 0.0   // 0
  };

  glm::vec3 pts[5] = {
    glm::vec3(-side2, -side2, 0.0),  // 0
    glm::vec3(side2, -side2, 0.0),  // 1
    glm::vec3(side2,  side2, 0.0),  // 2
    glm::vec3(-side2, side2, 0.0),  // 3
    glm::vec3(0.0, 0.0, _height)
  };

  glm::vec3 triNormals[4];
  triNormals[0] = glm::cross(pts[4]-pts[2], pts[4]-pts[3]);
  triNormals[1] = glm::cross(pts[4]-pts[1], pts[4]-pts[2]);
  triNormals[2] = glm::cross(pts[4]-pts[0], pts[4]-pts[1]);
  triNormals[3] = glm::cross(pts[4]-pts[3], pts[4]-pts[0]);

  std::vector<GLfloat> n = {
    // Base
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f
  };

  for (int i = 0; i < 4; i++) {
    for (int k = 0; k < 3; k++) {
      for (int j = 0; j < 3; j++) {
        n.push_back(triNormals[i][j]);
      }
    }
  }

  std::vector<GLfloat> tex = {
    // Base
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    // Side 1
    0.5f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    // Side 2
    0.5f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    // Side 3
    0.5f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    // Side 4
    0.5f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f
  };

  std::vector<GLuint> el = {
    0, 3, 2, 0, 2, 1,
    4, 5, 6,
    7, 8, 9,
    10, 11, 12,
    13, 14, 15,
  };

  initBuffers(&el, &p, &n, &tex);
}

}  // namespace agl
