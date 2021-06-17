// copyright 2020, Savvy Sine, Aline Normoyle

#ifndef AGL_RENDERER_H_
#define AGL_RENDERER_H_

#include <vector>
#include <list>
#include <string>
#include <map>
#include "agl/agl.h"
#include "agl/aglm.h"
#include "agl/image.h"
#include "agl/mesh.h"

namespace agl {

/**
 * @brief Mode for combining colors when drawing
 *
 * The alpha component controls how colors will be combined based on the
 * current mode
 *
 * * *DEFAULT* Ignore alpha and draw all objects as opaque
 * * *ADD* Add colors using formula: cSrc + c * c.alpha
 * * *ALPHA* Blend colors using formula: cSrc * alpha + c * (1 - c.alpha)
 * @verbinclude sprites.cpp
 */
enum BlendMode {
  DEFAULT,
  ADD,
  ALPHA};

/**
 * @brief The Renderer class draws meshes to the screen using shaders
 */
class Renderer {
 public:
  Renderer();
  virtual ~Renderer();

  /**
   * @brief Initialize this class for drawing.
   *
   * This method must be called before any draw or shader calls.
   * Window calls this method automatically. Users should not call this method.
   */
  void init();

  /**
   * @brief Cleanup all resources used for drawing.
   *
   * This method must be called before any draw or shader calls.
   * Window calls this method automatically. Users should not call this method.
   */
  void cleanup();

  /**
   * @brief Return whether the Renderer is ready for drawing
   * @return Returns true if initialized; false otherwise
   */
  bool initialized() const;

  /** @name Projections and view
   */
  ///@{
  /**
   * @brief Set the current projection to a perspective view
   * @param fovRadians The field of view
   * @param aspect The aspect ratio (width divided by height) of the screen
   * @param near The distance to the near plane from the camera
   * @param far The distance to the far plane from the camera
   *
   * If you are using Renderer from the Window class, you should call 
   * Window::perspective instead of this method.
   *
   * Perspective projections foreshorten objects should that closer objects are
   * larger than further objects.  Only objects within the view volume will be
   * drawn.  For example, objects a distance further from the *far* plane will
   * not be visible.
   *
   * The current shader should define the following uniform
   * variables
   *
   * *uniform mat4 MVP* A 4x4 matrix containing the product of projection *
   * view * modelTransform
   */
  void perspective(float fovRadians,
      float aspect, float near, float far);

  /**
   * @brief Set the current projection to an orthographic view
   * @param minx The left side of the projection
   * @param maxx The right side of the projection
   * @param miny The bottom side of the projection
   * @param maxy The top side of the projection
   * @param minz The back side of the projection
   * @param maxz The front side of the projection
   *
   * If you are using Renderer from the Window class, you should call 
   * Window::ortho instead of this method.
   *
   * An orthographic projection maintains parallel lines. All objects maintain
   * their size regardless of distance to the camera. Only objects within the
   * extents of the orthographic cuboid volume will be drawn.
   *
   * The current shader should define the following uniform variables
   *
   * *uniform mat4 MVP* A 4x4 matrix containing the product of projection *
   * view * modelTransform
   */
  void ortho(float minx, float maxx,
      float miny, float maxy, float minz, float maxz);

  /**
   * @brief Set the camera position and orientation
   * @param lookfrom The position of the camera (default is vec3(0,0,2))
   * @param lookat The target the camera is facing (default is vec3(0,0,0))
   * @param up The "up" direction of the camera (typically vec3(0,1,0))
   *
   * If you are using Renderer from the Window class, you should call 
   * Window::lookAt instead of this method.
   * NOTE: lookfrom and lookat should never be equal!
   *
   * The current shader should define the following uniform variables
   *
   * - *uniform mat4 MVP* A 4x4 matrix containing the product of projection *
   * view * modelTransform
   * - *uniform mat4 ModelViewMatrix* A 4x4 matrix containing the product of
   * view * modelTransform
   * - *uniform mat4 NormalMatrix* A 3x3 matrix for transforming normals.
   */
  void lookAt(const glm::vec3& lookfrom,
        const glm::vec3& lookat,
        const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

  /**
   * @brief Get the current camera position
   *
   * @verbinclude confetti.cpp
   */
  glm::vec3 cameraPosition() const;

  /**
   * @brief Get the current projection matrix
   *
   * @verbinclude select_drag.cpp
   */
  glm::mat4 projectionMatrix() const { return _projectionMatrix; }

  /**
   * @brief Get the current view matrix
   *
   * @verbinclude select_drag.cpp
   */
  glm::mat4 viewMatrix() const { return _viewMatrix; }
  ///@}

  /** @name Shaders
   */
  ///@{
  /**
   * @brief Load a GLSL shader from files
   * @param name A nickname for the shader to be used in beginShader()
   * @param vs The vertex shader file name
   * @param vs The fragment shader file name
   *
   * Shaders should be loaded in setup() The default shader is "phong". The
   * renderer automatically loads shaders for "phong", "sprites", and
   * "cubemap". Paths are relative to the directory from which you run your
   * application.
   * @see beginShader
   */
  void loadShader(const std::string& name,
      const std::string& vs, const std::string& fs);

  /**
   * @brief Set active shader to use for rendering.
   *
   * One shadr must always be enabled for drawing to work. By default, "phong"
   * is enabled.  The shader needs to be already loaded (in setup) using
   * `loadShader` The renderer automatically loads shaders for "phong",
   * "sprites", and "cubemap".
   *
   * Usage
   * ```
   * beginShader("myShader");
   * // draw primitives
   * endShader();
   * ```
   * @see loadShader
   */
  void beginShader(const std::string& shaderName);

  /**
   * @brief Clear active shader to use for rendering.
   *
   * @see beginShader
   */
  void endShader();

  /**
   * @brief Clear all active shaders
   *
   * Resets all active shaders (e.g. those for which beginShader has been
   * called but endShader() hasn't). This function is called by Window at the
   * end up each frame (users shouldn't need to call this function)
   * @see endShader
   */
  void cleanupShaders();

  /**
   * @brief Set a uniform parameter in the currently active shader
   *
   * The name should match the name of the uniform parameter. Overrides
   * correspond to different uniform variable types (which should also match).
   *
   * @verbinclude uniforms.cpp
   */
  void setUniform(const std::string& name, float x, float y, float z);

  /**
   * @copydoc setUniform(const std::string&,float,float,float)
   */
  void setUniform(const std::string& name, float x, float y, float z, float w);

  /**
   * @copydoc setUniform(const std::string&,float,float,float)
   */
  void setUniform(const std::string& name, const glm::vec2 &v);

  /**
   * @copydoc setUniform(const std::string&,float,float,float)
   */
  void setUniform(const std::string& name, const glm::vec3 &v);

  /**
   * @copydoc setUniform(const std::string&,float,float,float)
   */
  void setUniform(const std::string& name, const glm::vec4 &v);

  /**
   * @copydoc setUniform(const std::string&,float,float,float)
   */
  void setUniform(const std::string& name, const glm::mat4 &m);

  /**
   * @copydoc setUniform(const std::string&,float,float,float)
   */
  void setUniform(const std::string& name, const glm::mat3 &m);

  /**
   * @copydoc setUniform(const std::string&,float,float,float)
   */
  void setUniform(const std::string& name, float val);

  /**
   * @copydoc setUniform(const std::string&,float,float,float)
   */
  void setUniform(const std::string& name, int val);

  /**
   * @copydoc setUniform(const std::string&,float,float,float)
   */
  void setUniform(const std::string& name, bool val);

  /**
   * @copydoc setUniform(const std::string&,float,float,float)
   */
  void setUniform(const std::string& name, GLuint val);

  /**
   * @brief Set a uniform sampler parameter in the currently active shader
   *
   * The name should match the name of the uniform parameter. The texture
   * should already be loaded. The textureName should match the key given when
   * the texture is loaded.
   *
   * @see loadTexture
   * @verbinclude sprites.cpp
   */
  void texture(const std::string& uniformName, const std::string& textureName);

  /**
   * @brief Set a uniform sampler parameter in the currently active shader
   *
   * The name should match the name of the uniform parameter. The cubemap
   * should already be loaded. The textureName should match the key given when
   * the texture is loaded.
   *
   * @see loadCubemap
   * @verbinclude cubemap.cpp
   */
  void cubemap(const std::string& uniformName, const std::string& texName);

  /** @name Loading textures
   * @brief Textures should typically be loaded from setup()
   */
  ///@{
  /**
   * @brief Load a texture from a file
   *
   * @verbinclude sprites.cpp
   */
  void loadTexture(const std::string& name,
      const std::string& filename, int slot);

  /**
   * @brief Load a texture from an Image
   */
  void loadTexture(const std::string& name, const Image& img, int slot);

  /**
   * @brief Load a cube map
   */
  void loadCubemap(const std::string& name, const std::string& dir, int slot);

  /**
   * @brief Load a cube map
   */
  void loadCubemap(const std::string& name,
      const std::vector<std::string>& names, int slot);

  /**
   * @brief Load a cube map
   */
  void loadCubemap(const std::string& name,
      const std::vector<Image>& images, int slot);
  ///@}

  // drawing - positioning
  /** @name Positioning
   */
  ///@{
  void push();
  void pop();
  void identity();
  void scale(const glm::vec3& xyz);
  void translate(const glm::vec3& xyz);
  void rotate(float angleRad, const glm::vec3& axis);
  void transform(const glm::mat4& trs);
  ///@}

  /**
   * @brief Mode for combining colors when drawing
   *
   * The alpha component controls how colors will be combined based on the
   * current mode. Transparent objects should be drawn back to front in
   * relation to the camera.
   *
   * * *DEFAULT* Ignore alpha and draw all objects as opaque
   * * *ADD* Add colors using formula: cSrc + c * c.alpha
   * * *ALPHA* Blend colors using formula: cSrc * alpha + c * (1 - c.alpha)
   * @verbinclude sprites.cpp
   */
  void blendMode(BlendMode mode);

  /** @name Drawing
   */
  ///@{
  /**
   * @brief Draws a sprite using a point billboard
   * @param pos The location of the center of the sprite
   * @param color The color of the sprite
   * @param size The size (width/height) of the billboard
   *
   * @verbinclude sprites.cpp
   */
  void sprite(const glm::vec3& pos, const glm::vec4& color, float size);

  /**
   * @brief Draws a sprite using a point billboard
   * @param p1 The location of the first point
   * @param p2 The location of the second point
   * @param c1 The color of the first point
   * @param c2 The color of the second point
   *
   */
  void line(const glm::vec3& p1, const glm::vec3& p2,
      const glm::vec3& c1, const glm::vec3& c2);

  /**
   * @brief Draws a sphere centered at the origin with radius 0.5
   *
   * @verbinclude sphere.cpp
   */
  void sphere();

  /**
   * @brief Draws a cube centered at the origin with width, height, and depth
   * equal to 1.0
   *
   * @verbinclude rotate.cpp
   */
  void cube();

  /**
   * @brief Draws a cone centered at the origin, with the tip towards +Z
   *
   * @verbinclude transform.cpp
   */
  void cone();

  /**
   * @brief Draws a teapot with largest side with width 1
   *
   */
  void teapot();

  /**
   * @brief Draws a plane
   *
   */
  void plane();

  /**
   * @brief Draws a teapot with largest side with width 1
   *
   */
  void cylinder();

  /**
   * @brief Draws a capsule with endpoints at (0,0,0) and (0,0,1).
   * The cap radius is 0.25 and the width is 0.5.
   *
   */
  void capsule();

  /**
   * @brief Draws a torus
   *
   */
  void torus();

  /**
   * @brief Draws a skybox (typically with a cubemap)
   *
   * @verbinclude cubemap.cpp
   */
  void skybox(float size = 10.0);

  /**
   * @brief Draws a custom mesh
   *
   * Meshes can be loaded from a file or created procedurally.
   * Subclasses of leMesh should minimally define positions, normals, and
   * indices. Texture (UV) coordinates and tangents may also be defined.
   *
   * @see TriangleMesh
   * @see LineMesh
   * @see PointMesh
   */
  void mesh(const Mesh& m);
  ///@}

 private:
  void initBillboards();
  void initLines();
  void initMesh();

 private:
  bool _initialized;

  // textures
  struct Texture {
    GLuint texId;
    int slot;
  };
  std::map<std::string, Texture> _textures;

  // shaders
  class Shader* _currentShader;
  std::map<std::string, class Shader*> _shaders;
  std::list<Shader*> _shaderStack;

  // matrix stack
  std::list<glm::mat4> _stack;
  glm::mat4 _trs;

  // perspective and view
  glm::mat4 _projectionMatrix;
  glm::mat4 _viewMatrix;
  glm::vec3 _lookfrom;

  // default meshes
  class Cube* _cube;
  class Cylinder* _cone;
  class Capsule* _capsule;
  class Cylinder* _cylinder;
  class Teapot* _teapot;
  class Torus* _torus;
  class Plane* _plane;
  class Sphere* _sphere;
  class SkyBox* _skybox;

  // Quad
  GLuint mBBVboPosId;
  GLuint mBBVaoId;

  // Line
  GLuint mVboLinePosId;
  GLuint mVboLineColorId;
  GLuint mVaoLineId;
};

}  // namespace agl
#endif  // AGL_RENDERER_H_
