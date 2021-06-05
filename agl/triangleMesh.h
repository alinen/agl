// From OpenGL 4.0 Shading language cookbook (David Wolf 2011)
#pragma once

#include <vector>
#include "AGL.h"

namespace agl {
   class TriangleMesh
   {
   protected:

      GLuint nVerts;     // Number of vertices
      GLuint vao;        // The Vertex Array Object

      // Vertex buffers
      std::vector<GLuint> buffers;

      virtual void initBuffers(
         std::vector<GLuint>* indices,
         std::vector<GLfloat>* points,
         std::vector<GLfloat>* normals,
         std::vector<GLfloat>* texCoords = nullptr,
         std::vector<GLfloat>* tangents = nullptr
      );

      virtual void deleteBuffers();

   public:
      virtual ~TriangleMesh();
      virtual void render() const;
      GLuint getVao() const { return vao; }
   };
}
