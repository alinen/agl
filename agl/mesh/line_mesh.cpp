// Copyright, 2020, Savvy Sine, Aline Normoyle
#include "agl/mesh/line_mesh.h"
#include <iostream>

using glm::vec4;

namespace agl {

void LineMesh::render() const {
  if (!_initialized) const_cast<LineMesh*>(this)->init();
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

  glDrawArrays(GL_LINES, 0, _nVerts * 3);
  glBindVertexArray(0);
}

}  // namespace agl
