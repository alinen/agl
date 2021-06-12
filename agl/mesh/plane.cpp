// Copyright 2020, Savvy Sine, Aline Normoyle
#include "agl/mesh/plane.h"
#include <cstdio>
#include <vector>
#include <cmath>

namespace agl {

Plane::Plane(float xsize, float zsize,
    int xdivs, int zdivs, float smax, float tmax) {
  _xsize = xsize;
  _zsize = zsize;
  _xdivs = xdivs;
  _zdivs = zdivs;
  _smax = smax;
  _tmax = tmax;
}

void Plane::init() {
  // From: OpenGL 4.0 Shading language cookbook (David Wolf 2011)
  std::vector<GLfloat> p(3 * (_xdivs + 1) * (_zdivs + 1));
  std::vector<GLfloat> n(3 * (_xdivs + 1) * (_zdivs + 1));
  std::vector<GLfloat> tex(2 * (_xdivs + 1) * (_zdivs + 1));
  std::vector<GLuint> el(6 * _xdivs * _zdivs);

  float x2 = _xsize / 2.0f;
  float z2 = _zsize / 2.0f;
  float iFactor = static_cast<float>(_zsize / _zdivs);
  float jFactor = static_cast<float>(_xsize / _xdivs);
  float texi = _smax / _zdivs;
  float texj = _tmax / _xdivs;
  float x, z;
  int vidx = 0, tidx = 0;
  for (int i = 0; i <= _zdivs; i++) {
    z = iFactor * i - z2;
    for (int j = 0; j <= _xdivs; j++) {
      x = jFactor * j - x2;
      p[vidx] = x;
      p[vidx+1] = 0.0f;
      p[vidx+2] = z;

      n[vidx] = 0.0f;
      n[vidx+1] = 1.0f;
      n[vidx+2] = 0.0f;

      tex[tidx] = j * texi;
      tex[tidx+1] = i * texj;

      vidx += 3;
      tidx += 2;
    }
  }

  GLuint rowStart, nextRowStart;
  int idx = 0;
  for (int i = 0; i < _zdivs; i++) {
    rowStart = static_cast<GLuint> (i * (_xdivs+1));
    nextRowStart = static_cast<GLuint> ((i+1) * (_xdivs+1));
    for (int j = 0; j < _xdivs; j++) {
      el[idx] = rowStart + j;
      el[idx+1] = nextRowStart + j;
      el[idx+2] = nextRowStart + j + 1;
      el[idx+3] = rowStart + j;
      el[idx+4] = nextRowStart + j + 1;
      el[idx+5] = rowStart + j + 1;
      idx += 6;
    }
  }

  initBuffers(&el, &p, &n, &tex);
}

}  // namespace agl
