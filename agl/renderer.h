// Copyright 2020, Savvy Sine, alinen

#ifndef AGL_RENDERER_H_
#define AGL_RENDERER_H_

#include <vector>
#include <string>
#include <map>
#include "agl/agl.h"
#include "agl/aglm.h"
#include "agl/triangle_mesh.h"

namespace agl {

enum BlendMode {DEFAULT, ADD, ALPHA};

class Renderer {
 public:
  Renderer();
  ~Renderer();

  void init();
  void cleanup();
  bool initialized() const;

  // projection
  virtual void perspective(float fovRadians,
      float aspect, float near, float far);
  virtual void ortho(float minx, float maxx,
      float miny, float maxy, float minz, float maxz);

  // camera (eye point)
  virtual void lookAt(const glm::vec3& lookfrom, const glm::vec3& lookat);
  glm::vec3 cameraPosition() const;

  // shader
  void loadShader(const std::string& name, 
      const std::string& vs, const std::string& fs);

  void beginShader(const std::string& shaderName); 
  void endShader();

  void setUniform(const char *name, float x, float y, float z);
  void setUniform(const char *name, float x, float y, float z, float w);
  void setUniform(const char *name, const glm::vec2 &v);
  void setUniform(const char *name, const glm::vec3 &v);
  void setUniform(const char *name, const glm::vec4 &v);
  void setUniform(const char *name, const glm::mat4 &m);
  void setUniform(const char *name, const glm::mat3 &m);
  void setUniform(const char *name, float val);
  void setUniform(const char *name, int val);
  void setUniform(const char *name, bool val);
  void setUniform(const char *name, GLuint val);

  // textures
  GLuint loadTexture(const std::string& imageName);
  GLuint loadCubemap(const std::vector<std::string>& imageNames);

  // virtual void texture(GLuint textureId);  // NEW
  // virtual void color(const vec4& color);  // NEW

  // drawing - positioning
  
  // draw sprites
  void begin(GLuint textureId, BlendMode mode);
  void quad(const glm::vec3& pos, const glm::vec4& color, float size);
  void end();  // reset draw state

  // draw a mesh
  void sphere();
  void mesh(const glm::mat4& trs, const TriangleMesh& m);

  // draw the cubemap
  void skybox();

 protected:
  virtual void initBillboards();
  virtual void initCubemap();
  virtual void initMesh();

  virtual void blendMode(BlendMode mode);

 protected:
  bool _initialized;

  GLuint mBBVboPosId;  // quad rendering
  GLuint mBBVaoId;   // quad renderering

  GLuint mCubemap;  // skybox rendering
  class SkyBox* mSkybox;  // skybox rendering

  class Shader* _currentShader;
  std::map<std::string, class Shader*> _shaders;

  glm::mat4 mProjectionMatrix;
  glm::mat4 mViewMatrix;
  glm::vec3 mLookfrom;

  // default meshes
  class Sphere* _sphere; 
};

}  // namespace agl
#endif  // AGL_RENDERER_H_
