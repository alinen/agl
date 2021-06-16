// Copyright, 2020, Savvy Sine, Aline Normoyle
#include "agl/mesh.h"
#include <iostream>

using glm::vec4;

namespace agl {

void Mesh::initBuffers(
  std::vector<GLfloat> * points,
  std::vector<GLfloat> * normals,
  std::vector<GLfloat> * texCoords,
  std::vector<GLfloat> * tangents
) {
  if (_initialized) return;

  // Must have data for points
  if (points == nullptr) {
    std::cout << "initBuffers: points should not be null\n";
    return;
  }

  _initialized = true;
  _hasUV = (texCoords != nullptr);
  _nVerts = points->size() / 3;  // assumes xyz positions

  GLuint type = GL_STATIC_DRAW;
  if (_isDynamic) {
    type = GL_DYNAMIC_DRAW;
    _data[POSITION] = *(points);
    if (normals != nullptr) _data[NORMAL] = *normals;
    if (texCoords != nullptr) _data[UV] = *texCoords;
    if (tangents != nullptr) _data[TANGENT] = *tangents;
  }

  // The base mesh does not use an indexed buffer but subclasses might
  // Reserve the first ID for the index buffer, so that buffers are stored
  // similarly for all subclasses
  _buffers.push_back(0);

  // Based on OpenGL 4.0 Shading language cookbook (David Wolf)
  GLuint posBuf = 0, normBuf = 0, tcBuf = 0, tangentBuf = 0;
  glGenBuffers(1, &posBuf);
  _buffers.push_back(posBuf);
  glBindBuffer(GL_ARRAY_BUFFER, posBuf);
  glBufferData(GL_ARRAY_BUFFER,
      points->size() * sizeof(GLfloat), points->data(), type);

  if (normals != nullptr) {
    glGenBuffers(1, &normBuf);
    _buffers.push_back(normBuf);
    glBindBuffer(GL_ARRAY_BUFFER, normBuf);
    glBufferData(GL_ARRAY_BUFFER,
        normals->size() * sizeof(GLfloat), normals->data(), type);
  }

  if (texCoords != nullptr) {
    glGenBuffers(1, &tcBuf);
    _buffers.push_back(tcBuf);
    glBindBuffer(GL_ARRAY_BUFFER, tcBuf);
    glBufferData(GL_ARRAY_BUFFER,
        texCoords->size() * sizeof(GLfloat), texCoords->data(), type);
  }

  if (tangents != nullptr) {
    glGenBuffers(1, &tangentBuf);
    _buffers.push_back(tangentBuf);
    glBindBuffer(GL_ARRAY_BUFFER, tangentBuf);
    glBufferData(GL_ARRAY_BUFFER,
        tangents->size() * sizeof(GLfloat), tangents->data(), type);
  }

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  // Position
  glBindBuffer(GL_ARRAY_BUFFER, posBuf);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);  // Vertex position

  // Normal
  if (normals != nullptr) {
    glBindBuffer(GL_ARRAY_BUFFER, normBuf);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);  // Normal
  }

  // Tex coords
  if (texCoords != nullptr) {
    glBindBuffer(GL_ARRAY_BUFFER, tcBuf);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);  // Tex coord
  }

  if (tangents != nullptr) {
    glBindBuffer(GL_ARRAY_BUFFER, tangentBuf);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(3);  // Tangents
  }

  glBindVertexArray(0);
}

Mesh::~Mesh() {
  deleteBuffers();
}

void Mesh::deleteBuffers() {
  if (_buffers.size() > 0) {
    glDeleteBuffers((GLsizei)_buffers.size(), _buffers.data());
    _buffers.clear();
  }

  if (_vao != 0) {
    glDeleteVertexArrays(1, &_vao);
    _vao = 0;
  }
}

void Mesh::setIsDynamic(bool on) {
  assert(_initialized == false);
  _isDynamic = on;
}

void Mesh::setVertexData(VertexAttribute type,
  int vertexId, const vec4& pos) {
  assert(vertexId >= 0 && vertexId < _nVerts);

  int stride = _data[type].size() / _nVerts;
  assert(stride > 0);

  if (stride >= 1) {
    _data[type][vertexId*stride + 0] = pos.x;
  }
  if (stride >= 2) {
    _data[type][vertexId*stride + 1] = pos.y;
  }
  if (stride >= 3) {
    _data[type][vertexId*stride + 2] = pos.z;
  }
  if (stride >= 4) {
    _data[type][vertexId*stride + 3] = pos.w;
  }
}

vec4 Mesh::vertexData(VertexAttribute type, int vertexId) const {
  assert(vertexId >= 0 && vertexId < _nVerts);

  int stride = _data[type].size() / _nVerts;
  assert(stride > 0);

  vec4 value(0);
  if (stride >= 1) {
    value[0] = _data[type][vertexId*stride + 0];
  }
  if (stride >= 2) {
    value[1] = _data[type][vertexId*stride + 1];
  }
  if (stride >= 3) {
    value[2] = _data[type][vertexId*stride + 2];
  }
  if (stride >= 4) {
    value[3] = _data[type][vertexId*stride + 3];
  }
  return value;
}

}  //  namespace agl
