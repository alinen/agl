// Copyright 2020, Savvy Sine, alinen

#ifndef AGL_RENDERER_H_
#define AGL_RENDERER_H_

#include <vector>
#include <string>
#include <map>
#include "agl/agl.h"
#include "agl/aglm.h"
#include "agl/image.h"
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
  virtual void lookAt(const glm::vec3& lookfrom, 
        const glm::vec3& lookat,
        const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec3 cameraPosition() const;

  // shader
  void loadShader(const std::string& name, 
      const std::string& vs, const std::string& fs);

  void beginShader(const std::string& shaderName); 
  void endShader();

  void setUniform(const std::string& name, float x, float y, float z);
  void setUniform(const std::string& name, float x, float y, float z, float w);
  void setUniform(const std::string& name, const glm::vec2 &v);
  void setUniform(const std::string& name, const glm::vec3 &v);
  void setUniform(const std::string& name, const glm::vec4 &v);
  void setUniform(const std::string& name, const glm::mat4 &m);
  void setUniform(const std::string& name, const glm::mat3 &m);
  void setUniform(const std::string& name, float val);
  void setUniform(const std::string& name, int val);
  void setUniform(const std::string& name, bool val);
  void setUniform(const std::string& name, GLuint val);

  // textures
  void loadTexture(const std::string& name, 
      const std::string& filename, int slot);
  void loadTexture(const std::string& name, const Image& img, int slot);

  void loadCubemap(const std::string& name, const std::string& dir, int slot);
  void loadCubemap(const std::string& name, 
      const std::vector<std::string>& names, int slot);
  void loadCubemap(const std::string& name, 
      const std::vector<Image>& images, int slot);

  void texture(const std::string& uniformName, const std::string& textureName);
  void cubemap(const std::string& uniformName, const std::string& texName);

  // drawing - positioning
  // drawing - settings
  void blendMode(BlendMode mode);
  
  // draw sprites
  void quad(const glm::vec3& pos, const glm::vec4& color, float size);

  // draw a mesh
  void sphere();
  void mesh(const glm::mat4& trs, const TriangleMesh& m);

  // draw the cubemap
  void skybox(float size = 10.0);

 protected:
  virtual void initBillboards();
  virtual void initMesh();

 protected:
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

  // perspective and view 
  glm::mat4 mProjectionMatrix;
  glm::mat4 mViewMatrix;
  glm::vec3 mLookfrom;

  // default meshes
  class Sphere* _sphere; 
  class SkyBox* _skybox; 

  // Quad
  GLuint mBBVboPosId; 
  GLuint mBBVaoId;
};

}  // namespace agl
#endif  // AGL_RENDERER_H_
