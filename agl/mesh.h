// Copyright, 2020, Savvy Sine, Aline Normoyle
#ifndef AGL_MESH_H_
#define AGL_MESH_H_

#include <vector>
#include "agl/agl.h"
#include "agl/aglm.h"

namespace agl {

/**
 * @brief Base class for meshes
 * 
 * @see TriangleMesh 
 * @see PointMesh 
 * @see LineMesh 
 */
class Mesh {
 public:
  virtual ~Mesh();

  /**
   * @brief Draw this mesh
   *
   * Typically, users do not need to call this function. It is called from 
   * Renderer.
   * @see Renderer::mesh(const Mesh&)
   * @see TriangleMesh::render()
   * @see PointMesh::render()
   * @see LineMesh::render()
   */ 
  virtual void render() const = 0;

  /**
   * @brief Return the vertex array object corresponding to this mesh
   */ 
  GLuint vao() const { return _vao; }

  /**
   * @brief Return whether this mesh has UV coordinates defined.
   */ 
  bool hasUV() const { return _hasUV; }

  /**
   * @brief Query whether or not this is a dynamic mesh
   * 
   * Dynamic meshes can change vertex quantities (e.g. position, normal, etc.)
   * at runtime. Dynamic meshes require more memory and are generally slower.
   * Meshes are static (e.g. assumed not to change) by default.
   *
   * To create a dynamic mesh, subclass this class and set _isDynamic to true
   * in the constructor.
   * 
   * @verbinclude undulate.cpp
   * @see setIsDynamic(bool)
   */
  bool isDynamic() const { return _isDynamic; }

 protected:
  GLuint _nVerts = 0;      // Number of unique vertices
  GLuint _vao = 0;         // The Vertex Array Object
  bool _hasUV = false;
  bool _isDynamic = false;
  bool _initialized = false;
  std::vector<GLuint> _buffers;   // vertex buffers
  std::vector<GLfloat> _data[5];  // State for dynamic meshes
  enum VertexAttribute {
    INDEX = 0,
    POSITION,
    NORMAL,
    UV,
    TANGENT,
    NUM_ATTRIBUTES
  };

  /**
   * @brief Get the number of vertices
   */
  int numVertices() const { return _nVerts; }

  /**
   * @brief Set vertex properties
   * @param type A supported vertex attribute, such as POSITION, NORMAL, etc
   * @param vertexId The id of the vertex (should be between 0 and numVertices) 
   * @param data The data to set
   * 
   * This function only works for dynamic meshes.
   * This function takes a vec4 but depending on the vertex attribute only a 
   * subset of the given values will be used, depending on the data set during
   * initialization. For example, texture coordinates typically only need two
   * coordinates and so only the first xy values of data will be used.
   *
   * @verbinclude undulate.cpp
   * @see numVertices()
   * @see vertexAttribute()
   * @see setIsDynamic()
   */
  void setVertexData(VertexAttribute type, int vertexId, const glm::vec4& data);

  /**
   * @brief Get vertex properties
   * @param type A supported vertex attribute, such as POSITION, NORMAL, etc
   * @param vertexId The id of the vertex (should be between 0 and numVertices) 
   * @return The data for the given vertex if the mesh is dynamic, and 
   * vec4(0) otherwise
   * 
   * This function only works for dynamic meshes.
   * This function takes a vec4 but depending on the vertex attribute only a 
   * subset of the given values will be used, depending on the data set during
   * initialization. For example, texture coordinates typically only need two
   * coordinates and so only the first xy values of data will be used.
   *
   * @verbinclude undulate.cpp
   * @see numVertices()
   * @see setVertexAttribute()
   * @see setIsDynamic()
   */
  glm::vec4 vertexData(VertexAttribute type, int vertexId) const;

  /**
   * @brief Set whether or not this is a dynamic mesh
   * 
   * Dynamic meshes can change vertex quantities (e.g. position, normal, etc.)
   * at runtime. Dynamic meshes require more memory and are generally slower.
   * Meshes are static (e.g. assumed not to change) by default.
   *
   * To create a dynamic mesh, subclass this class and call setIsDynamic(true)
   * in the constructor. setIsDynamic must be called before the mesh is 
   * initialized and cannot be changed afterwards.
   * 
   * @verbinclude undulate.cpp
   * @see isDynamic()
   */
  virtual void setIsDynamic(bool on);

  /**
   * @brief Override init to specifiy vertex data for the mesh
   *
   * init() will be called automatically the first time render() is called.
   * Overridden implementations of init() should call initBuffers.
   * @see initBuffers(std::vector<GLuint>*,
   *     std::vector<GLfloat>*, std::vector<GLfloat>*, 
   *     std::vector<GLfloat>*, std::vector<GLfloat>*) 
   */
  virtual void init() = 0;

  /**
   * @brief Call initBuffers from init() to set the data for this mesh
   *
   * @see init()
   * @see setIsDynamic(bool)
   */
  void initBuffers(
    std::vector<GLfloat>* points,
    std::vector<GLfloat>* normals,
    std::vector<GLfloat>* texCoords = nullptr,
    std::vector<GLfloat>* tangents = nullptr);

  virtual void deleteBuffers();
};

}  // namespace agl
#endif  // AGL_MESH_H_
