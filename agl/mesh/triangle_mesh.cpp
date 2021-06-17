// Copyright, 2020, Savvy Sine, Aline Normoyle
#include "agl/mesh/triangle_mesh.h"
#include <iostream>

using glm::vec4;

namespace agl {

void TriangleMesh::initBuffers(
  std::vector<GLuint> * indices,
  std::vector<GLfloat> * points,
  std::vector<GLfloat> * normals,
  std::vector<GLfloat> * texCoords,
  std::vector<GLfloat> * tangents
) {
  if (_initialized) return;

  // Must have data for indices, points, and normals
  if (indices == nullptr || points == nullptr || normals == nullptr) {
    std::cout <<
        "initBuffers: indices, points, and normals should not be null\n";
    return;
  }

  _initialized = true;
  _hasUV = (texCoords != nullptr);
  _nIndices = (GLuint)indices->size();
  _nVerts = points->size() / 3;  // assumes xyz positions

  GLuint type = GL_STATIC_DRAW;
  if (_isDynamic) {
    type = GL_DYNAMIC_DRAW;
    _data[POSITION] = *(points);
    _data[NORMAL] = *(normals);
    if (texCoords != nullptr) _data[UV] = *texCoords;
    if (tangents != nullptr) _data[TANGENT] = *tangents;
  }

  // Based on OpenGL 4.0 Shading language cookbook (David Wolf)
  GLuint indexBuf = 0, posBuf = 0, normBuf = 0, tcBuf = 0, tangentBuf = 0;
  glGenBuffers(1, &indexBuf);
  _buffers.push_back(indexBuf);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuf);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
      indices->size() * sizeof(GLuint), indices->data(), type);

  glGenBuffers(1, &posBuf);
  _buffers.push_back(posBuf);
  glBindBuffer(GL_ARRAY_BUFFER, posBuf);
  glBufferData(GL_ARRAY_BUFFER,
      points->size() * sizeof(GLfloat), points->data(), type);

  glGenBuffers(1, &normBuf);
  _buffers.push_back(normBuf);
  glBindBuffer(GL_ARRAY_BUFFER, normBuf);
  glBufferData(GL_ARRAY_BUFFER,
      normals->size() * sizeof(GLfloat), normals->data(), type);

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

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuf);

  // Position
  glBindBuffer(GL_ARRAY_BUFFER, posBuf);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);  // Vertex position

  // Normal
  glBindBuffer(GL_ARRAY_BUFFER, normBuf);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);  // Normal

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

void TriangleMesh::render() const {
  if (!_initialized) const_cast<TriangleMesh*>(this)->init();
  if (_vao == 0) return;

  glBindVertexArray(_vao);

  if (_isDynamic) {
    for (int i = 1; i < NUM_ATTRIBUTES; i++) {
      if (_data[i].size() > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, _buffers[i]);
        glBufferData(GL_ARRAY_BUFFER, _data[i].size() * sizeof(GLfloat),
            _data[i].data(), GL_DYNAMIC_DRAW);
      }
    }
  }

  glDrawElements(GL_TRIANGLES, _nIndices, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

}  //  namespace agl
