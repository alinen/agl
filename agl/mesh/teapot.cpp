#include "agl/mesh/teapot.h"
#include <cstdio>
#include "agl/aglm.h"
#include "agl/mesh/teapotdata.h"

using glm::vec3;
using glm::mat4;
using glm::mat3;
using glm::vec4;

namespace agl {

Teapot::Teapot(int grid, const mat4 & lidTransform) {
  _grid = grid;
  _lidTransform = lidTransform;
}

void Teapot::init() {
  // From: OpenGL 4.0 Shading language cookbook (David Wolf 2011)
  int verts = 32 * (_grid + 1) * (_grid + 1);
  int faces = _grid * _grid * 32;
  std::vector<GLfloat> p( verts * 3 );
  std::vector<GLfloat> n( verts * 3 );
  std::vector<GLfloat> tc( verts * 2 );
  std::vector<GLuint> el( faces * 6 );

  generatePatches( p, n, tc, el, _grid );
  moveLid(_grid, p, _lidTransform);
  fitUnitBox(p, n);
  initBuffers(&el, &p, &n, &tc);
}

void Teapot::fitUnitBox(std::vector<GLfloat>& p, std::vector<GLfloat>& n) {
  vec3 min, max;
  for (int i = 0; i < p.size(); i += 3) {
    float x = p[i+0]; 
    float y = p[i+1]; 
    float z = p[i+2]; 
    if (i == 0) {
      min = vec3(x, y, z);
      max = min;
    }
    if (x < min[0]) min[0] = x;
    if (y < min[1]) min[1] = y;
    if (z < min[2]) min[2] = z;

    if (x > max[0]) max[0] = x;
    if (y > max[1]) max[1] = y;
    if (z > max[2]) max[2] = z;
  }

  vec3 center = 0.5f * (max + min);
  vec3 bounds = (max - min);
  float scale = std::max(std::max(bounds[0], bounds[1]), bounds[2]);
  mat3 R = mat3(glm::angleAxis(glm::half_pi<float>(), vec3(-1, 0, 0))); 

  for (int i = 0; i < p.size(); i += 3) {
    vec3 pos = R * (vec3(p[i], p[i+1], p[i+2]) - center) / scale;
    vec3 nor = R * vec3(n[i], n[i+1], n[i+2]);

    p[i+0] = pos[0]; 
    p[i+1] = pos[1]; 
    p[i+2] = pos[2]; 

    n[i+0] = nor[0]; 
    n[i+1] = nor[1]; 
    n[i+2] = nor[2]; 
  }

}

// From: OpenGL 4.0 Shading language cookbook (David Wolf 2011)
void Teapot::generatePatches(
        std::vector<GLfloat> & p,
        std::vector<GLfloat> & n,
        std::vector<GLfloat> & tc,
        std::vector<GLuint> & el,
        int grid)
{
  std::vector<GLfloat> B(4*(grid+1));  // Pre-computed Bernstein basis functions
  std::vector<GLfloat> dB(4*(grid+1)); // Pre-computed derivitives of basis functions

  int idx = 0, elIndex = 0, tcIndex = 0;

  // Pre-compute the basis functions  (Bernstein polynomials)
  // and their derivatives
  computeBasisFunctions(B, dB, grid);

  // Build each patch
  // The rim
  buildPatchReflect(0, B, dB, p, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
  // The body
  buildPatchReflect(1, B, dB, p, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
  buildPatchReflect(2, B, dB, p, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
  // The lid
  buildPatchReflect(3, B, dB, p, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
  buildPatchReflect(4, B, dB, p, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
  // The bottom
  buildPatchReflect(5, B, dB, p, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
  // The handle
  buildPatchReflect(6, B, dB, p, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
  buildPatchReflect(7, B, dB, p, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
  // The spout
  buildPatchReflect(8, B, dB, p, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
  buildPatchReflect(9, B, dB, p, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
}

// From: OpenGL 4.0 Shading language cookbook (David Wolf 2011)
void Teapot::moveLid(int grid, std::vector<GLfloat> & p, const mat4 & lidTransform) {

    int start = 3 * 12 * (grid+1) * (grid+1);
    int end = 3 * 20 * (grid+1) * (grid+1);

    for( int i = start; i < end; i+=3 )
    {
        vec4 vert = vec4(p[i], p[i+1], p[i+2], 1.0f );
        vert = lidTransform * vert;
        p[i] = vert.x;
        p[i+1] = vert.y;
        p[i+2] = vert.z;
    }
}

// From: OpenGL 4.0 Shading language cookbook (David Wolf 2011)
void Teapot::buildPatchReflect(int patchNum,
                               std::vector<GLfloat> & B, std::vector<GLfloat> & dB,
                               std::vector<GLfloat> & v, std::vector<GLfloat> & n,
                               std::vector<GLfloat> & tc, std::vector<GLuint> & el,
                               int &index, int &elIndex, int &tcIndex, int grid,
                               bool reflectX, bool reflectY)
{
    vec3 patch[4][4];
    vec3 patchRevV[4][4];
    getPatch(patchNum, patch, false);
    getPatch(patchNum, patchRevV, true);

    // Patch without modification
    buildPatch(patch, B, dB, v, n, tc, el,
               index, elIndex, tcIndex, grid, mat3(1.0f), true);

    // Patch reflected in x
    if( reflectX ) {
        buildPatch(patchRevV, B, dB, v, n, tc, el,
                   index, elIndex, tcIndex, grid, mat3(vec3(-1.0f, 0.0f, 0.0f),
                                              vec3(0.0f, 1.0f, 0.0f),
                                              vec3(0.0f, 0.0f, 1.0f) ), false );
    }

    // Patch reflected in y
    if( reflectY ) {
        buildPatch(patchRevV, B, dB, v, n, tc, el,
                   index, elIndex, tcIndex, grid, mat3(vec3(1.0f, 0.0f, 0.0f),
                                              vec3(0.0f, -1.0f, 0.0f),
                                              vec3(0.0f, 0.0f, 1.0f) ), false );
    }

    // Patch reflected in x and y
    if( reflectX && reflectY ) {
        buildPatch(patch, B, dB, v, n, tc, el,
                   index, elIndex, tcIndex, grid, mat3(vec3(-1.0f, 0.0f, 0.0f),
                                              vec3(0.0f, -1.0f, 0.0f),
                                              vec3(0.0f, 0.0f, 1.0f) ), true );
    }
}

// From: OpenGL 4.0 Shading language cookbook (David Wolf 2011)
void Teapot::buildPatch(vec3 patch[][4],
                        std::vector<GLfloat> & B, std::vector<GLfloat> & dB,
                        std::vector<GLfloat> & v, std::vector<GLfloat> & n,
                        std::vector<GLfloat> & tc, std::vector<GLuint> & el,
                        int &index, int &elIndex, int &tcIndex, int grid, mat3 reflect,
                        bool invertNormal)
{
    int startIndex = index / 3;
    float tcFactor = 1.0f / grid;

    for( int i = 0; i <= grid; i++ )
    {
        for( int j = 0 ; j <= grid; j++)
        {
            vec3 pt = reflect * evaluate(i,j,B,patch);
            vec3 norm = reflect * evaluateNormal(i,j,B,dB,patch);
            if( invertNormal )
                norm = -norm;

            v[index] = pt.x;
            v[index+1] = pt.y;
            v[index+2] = pt.z;

            n[index] = norm.x;
            n[index+1] = norm.y;
            n[index+2] = norm.z;

            tc[tcIndex] = i * tcFactor;
            tc[tcIndex+1] = j * tcFactor;

            index += 3;
            tcIndex += 2;
        }
    }

    for( int i = 0; i < grid; i++ )
    {
        int iStart = i * (grid+1) + startIndex;
        int nextiStart = (i+1) * (grid+1) + startIndex;
        for( int j = 0; j < grid; j++)
        {
            el[elIndex] = iStart + j;
            el[elIndex+1] = nextiStart + j + 1;
            el[elIndex+2] = nextiStart + j;

            el[elIndex+3] = iStart + j;
            el[elIndex+4] = iStart + j + 1;
            el[elIndex+5] = nextiStart + j + 1;

            elIndex += 6;
        }
    }
}

// From: OpenGL 4.0 Shading language cookbook (David Wolf 2011)
void Teapot::getPatch( int patchNum, vec3 patch[][4], bool reverseV )
{
    for( int u = 0; u < 4; u++) {          // Loop in u direction
        for( int v = 0; v < 4; v++ ) {     // Loop in v direction
            if( reverseV ) {
                patch[u][v] = vec3(
                        TeapotData::cpdata[TeapotData::patchdata[patchNum][u*4+(3-v)]][0],
                        TeapotData::cpdata[TeapotData::patchdata[patchNum][u*4+(3-v)]][1],
                        TeapotData::cpdata[TeapotData::patchdata[patchNum][u*4+(3-v)]][2]
                        );
            } else {
                patch[u][v] = vec3(
                        TeapotData::cpdata[TeapotData::patchdata[patchNum][u*4+v]][0],
                        TeapotData::cpdata[TeapotData::patchdata[patchNum][u*4+v]][1],
                        TeapotData::cpdata[TeapotData::patchdata[patchNum][u*4+v]][2]
                        );
            }
        }
    }
}

// From: OpenGL 4.0 Shading language cookbook (David Wolf 2011)
void Teapot::computeBasisFunctions( std::vector<GLfloat> & B, std::vector<GLfloat> & dB, int grid ) {
    float inc = 1.0f / grid;
    for( int i = 0; i <= grid; i++ )
    {
        float t = i * inc;
        float tSqr = t * t;
        float oneMinusT = (1.0f - t);
        float oneMinusT2 = oneMinusT * oneMinusT;

        B[i*4 + 0] = oneMinusT * oneMinusT2;
        B[i*4 + 1] = 3.0f * oneMinusT2 * t;
        B[i*4 + 2] = 3.0f * oneMinusT * tSqr;
        B[i*4 + 3] = t * tSqr;

        dB[i*4 + 0] = -3.0f * oneMinusT2;
        dB[i*4 + 1] = -6.0f * t * oneMinusT + 3.0f * oneMinusT2;
        dB[i*4 + 2] = -3.0f * tSqr + 6.0f * t * oneMinusT;
        dB[i*4 + 3] = 3.0f * tSqr;
    }
}


// From: OpenGL 4.0 Shading language cookbook (David Wolf 2011)
vec3 Teapot::evaluate( int gridU, int gridV, std::vector<GLfloat> & B, vec3 patch[][4] )
{
    vec3 p(0.0f,0.0f,0.0f);
    for( int i = 0; i < 4; i++) {
        for( int j = 0; j < 4; j++) {
            p += patch[i][j] * B[gridU*4+i] * B[gridV*4+j];
        }
    }
    return p;
}

// From: OpenGL 4.0 Shading language cookbook (David Wolf 2011)
vec3 Teapot::evaluateNormal( int gridU, int gridV, std::vector<GLfloat> & B, std::vector<GLfloat> & dB, vec3 patch[][4] )
{
    vec3 du(0.0f,0.0f,0.0f);
    vec3 dv(0.0f,0.0f,0.0f);

    for( int i = 0; i < 4; i++) {
        for( int j = 0; j < 4; j++) {
            du += patch[i][j] * dB[gridU*4+i] * B[gridV*4+j];
            dv += patch[i][j] * B[gridU*4+i] * dB[gridV*4+j];
        }
    }

    vec3 norm = glm::cross(du, dv);
    if (glm::length(norm) != 0.0f) {
        norm = glm::normalize(norm);
    }

    return norm;
}
}
