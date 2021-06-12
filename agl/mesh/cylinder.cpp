// Copyright 2018, Savvy Sine, Aline Normoyle

#include "agl/mesh/cylinder.h"
#include <iostream>
#include <vector>
#include <cmath>
#include "agl/aglm.h"

namespace agl {

Cylinder::Cylinder(float r, float len, GLuint nSlices) {
  _r1 = r;
  _r2 = r;
  _length = len;
  _nSlices = nSlices;
}

Cylinder::Cylinder(float r1, float r2, float len, GLuint nSlices) {
  _r1 = r1;
  _r2 = r2;
  _length = len;
  _nSlices = nSlices;
}

void Cylinder::init() {
  computeMesh(_r1, _r2, _length, _nSlices);
}

void Cylinder::computeMesh(float r1, float r2, float len, GLuint nSlices) {
  int nVerts = (nSlices+1) * 2 + nSlices * 2;

  // num top triangles = nSlices
  // num bottom triangles = nSlices
  // num side triangles = nSlices * 2
  int elements = (nSlices) * 4 * 3;

  // Verts
  std::vector<GLfloat> p(3 * nVerts);
  // Normals
  std::vector<GLfloat> n(3 * nVerts);
  // Tex coords
  std::vector<GLfloat> tex(2 * nVerts);
  // Elements
  std::vector<GLuint> el(elements);

  // Generate positions and normals
  GLfloat theta;
  GLfloat thetaFac = glm::two_pi<float>() / nSlices;
  GLfloat hLen = 0.5 * len;
  GLuint offsets[4] = {
    0,
    nSlices+1,
    (nSlices+1)*2 - 1,
    (nSlices+1)*2 + nSlices - 1
  };

  GLuint idx = 0, tIdx = 0;
  for (GLuint i = 0; i <= nSlices; i++) {
    if (i == 0) {
      for (GLuint j = 0; j < 2; j++) {
        int offset = offsets[j];
        float z = j % 2 == 0? -hLen : hLen;
        p[idx+0+3*offset] = 0.0;
        p[idx+1+3*offset] = 0.0;
        p[idx+2+3*offset] = z;

        n[idx+0+3*offset] = 0.0;
        n[idx+1+3*offset] = 0.0;
        n[idx+2+3*offset] = j % 2 == 0? -1.0 : 1.0;

        tex[tIdx+0+2*offset] = 0.0;
        tex[tIdx+1+2*offset] = (z+hLen)/len;
      }
    } else {
      theta = (i-1) * thetaFac;
      float nx = cosf(theta);
      float ny = sinf(theta);

      float s = 0.5 * (theta / glm::pi<float>());

      for (GLuint j = 0; j < 4; j++) {
        int offset = offsets[j];
        float z = j % 2 == 0? -hLen : hLen;
        float r = j % 2 == 0? r1 : r2;
        float t = (z+hLen)/len;

        p[idx+0+3*offset] = r * nx;
        p[idx+1+3*offset] = r * ny;
        p[idx+2+3*offset] = z;

        n[idx+0+3*offset] = j < 2? 0.0 : nx;
        n[idx+1+3*offset] = j < 2? 0.0 : ny;  // todo: fix normal
        n[idx+2+3*offset] = j < 2? (j % 2 == 0? -1.0 : 1.0) : 0.0;

        tex[tIdx+0+2*offset] = s;
        tex[tIdx+1+2*offset] = t;
      }
    }

    idx += 3;
    tIdx += 2;
  }

  // Generate the element list
  idx = 0;
  for (GLuint i = 0; i < nSlices; i++) {
    el[idx+0] = 0;
    el[idx+1] = (i+1) % (nSlices) + 1;
    el[idx+2] = (i+1);

    el[idx+0+3*nSlices] = el[idx+0] + offsets[1];
    el[idx+1+3*nSlices] = el[idx+2] + offsets[1];
    el[idx+2+3*nSlices] = el[idx+1] + offsets[1];

    idx += 3;
  }


  int offset = nSlices * 2 + 2;
  idx = (nSlices)*2*3;
  for (GLuint i = 0; i < nSlices; i++) {
    int back1 = i + offset;
    int back2 = (i+1) % nSlices + offset;

    int front1 = back1 + nSlices;
    int front2 = back2 + nSlices;

    el[idx+0] = front1;
    el[idx+1] = back1;
    el[idx+2] = back2;

    el[idx+3] = front1;
    el[idx+4] = back2;
    el[idx+5] = front2;

    idx += 6;
  }

  initBuffers(&el, &p, &n, &tex);
}

}  // namespace agl
