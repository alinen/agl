// Copyright 2020, Savvy Sine, aline

#include "agl/renderer.h"
#include <fstream>
#include <sstream>
#include "agl/image.h"
#include "agl/shader.h"
#include "agl/sphere.h"
#include "agl/skybox.h"

namespace agl {

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;
using std::string;
using std::vector;

Renderer::Renderer() {
  _skybox = 0;
  _sphere = 0;
  _initialized = false;
}

Renderer::~Renderer() {
  cleanup();
}

void Renderer::cleanup() {
  delete _sphere;
  _sphere = 0;

  delete _skybox;
  _skybox = 0;

  for (auto it : _shaders) {
    delete it.second; 
  }
  _shaders.clear();
  _textures.clear();
  _initialized = false;
}

bool Renderer::initialized() const {
  return _initialized;
}


vec3 Renderer::cameraPosition() const {
  return mLookfrom;
}

void Renderer::init() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // setup default camera and projection
  float halfw = 1.0;
  float halfh = 1.0;
  ortho(-halfw, halfw, -halfh, halfh, -10.0f, 10.0f);
  lookAt(vec3(0, 0, 2), vec3(0, 0, 0));

  initBillboards();
  initCubemap();
  initMesh();

  _sphere = new Sphere(0.5f, 48, 48);
  _initialized = true;
}

void Renderer::initCubemap() {
  _skybox = new SkyBox(1);
  loadShader("cubemap", "../shaders/cubemap.vs", "../shaders/cubemap.fs");
}

void Renderer::initMesh() {
  loadShader("phong", "../shaders/phong.vs", "../shaders/phong.fs");

  // Set default parameters
  beginShader("phong");
  setUniform("Gamma", 0.8f);
  setUniform("Material.Ks", 1.0f, 1.0f, 1.0f);
  setUniform("Material.Kd", 0.4f, 0.6f, 1.0f);
  setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
  setUniform("Material.shininess", 80.0f);
  setUniform("Light.position", 100.0f, 100.0f, 100.0f, 1.0f);
  setUniform("Light.color", 1.0f, 1.0f, 1.0f);
  endShader();
}

void Renderer::initBillboards() {
  const float positions[] = {
     0.0f, 0.0f, 0.0f,
     1.0f, 0.0f, 0.0f,
     0.0f, 1.0f, 0.0f,

     1.0f, 0.0f, 0.0f,
     1.0f, 1.0f, 0.0f,
     0.0f, 1.0f, 0.0f,
  };

  glGenBuffers(1, &mBBVboPosId);
  glBindBuffer(GL_ARRAY_BUFFER, mBBVboPosId);
  glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), positions, GL_STATIC_DRAW);

  glGenVertexArrays(1, &mBBVaoId);
  glBindVertexArray(mBBVaoId);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, mBBVboPosId);  // bind before setting data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<GLubyte*>(0));

  loadShader("sprite", 
      "../shaders/billboard.vs",
      "../shaders/billboard.fs");
}

void Renderer::blendMode(BlendMode mode) {
  if (mode == ADD) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);  // Additive blend

  } else if (mode == ALPHA) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Alpha blend

  } else {
    glDisable(GL_BLEND);
  }
}

void Renderer::perspective(float fovRadians,
    float aspect, float near, float far) {
  mProjectionMatrix = glm::perspective(fovRadians, aspect, near, far);
}

void Renderer::ortho(float minx, float maxx,
    float miny, float maxy, float minz, float maxz) {
  mProjectionMatrix = glm::ortho(minx, maxx, miny, maxy, minz, maxz);
}

void Renderer::lookAt(const vec3& lookfrom, const vec3& lookat) {
  mLookfrom = lookfrom;
  mViewMatrix = glm::lookAt(lookfrom, lookat, vec3(0, 1, 0));
}

void Renderer::texture(const std::string& uniformName, const std::string& textureName) {
  assert(_textures.count(textureName) != 0);

  glBindTexture(GL_TEXTURE_2D, _textures[textureName].texId);
  setUniform(uniformName, _textures[textureName].slot);
}

void Renderer::quad(const glm::vec3& pos, const glm::vec4& color, float size) {
  assert(_initialized);
  assert(_currentShader == _shaders["sprite"]);

  setUniform("Offset", pos);
  setUniform("Color", color);
  setUniform("Size", size);

  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::cubemap(const std::string& uniformName, const std::string& textureName) {
  assert(_textures.count(textureName) != 0);

  glBindTexture(GL_TEXTURE_CUBE_MAP, _textures[textureName].texId);
  setUniform(uniformName, _textures[textureName].slot);
}

void Renderer::skybox(float size) {
  assert(_initialized);

  mat4 mvp = mProjectionMatrix * mViewMatrix * scale(mat4(1.0), vec3(size));
  setUniform("MVP", mvp);
  _skybox->render();
}

void Renderer::sphere() {
  mesh(mat4(1), *_sphere);
}

void Renderer::mesh(const mat4& trs, const TriangleMesh& mesh) {
  assert(_initialized);

  beginShader("phong");

  // GLuint timeParamId = glGetUniformLocation(mMShaderId, "time");

  mat4 mv = mViewMatrix * trs;
  mat4 mvp = mProjectionMatrix * mv;
  mat3 nmv = mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]));
  setUniform("MVP", mvp);  
  setUniform("ModelViewMatrix", mv); 
  setUniform("NormalMatrix", nmv); 

  mesh.render();
  endShader();
}

void Renderer::beginShader(const std::string& shaderName) {
  assert(_shaders.count(shaderName) != 0);

  _currentShader = _shaders[shaderName];
  _currentShader->use(); 

  if (shaderName == "sprite") {
    mat4 mvp = mProjectionMatrix * mViewMatrix;
    setUniform("MVP", mvp); 
    setUniform("CameraPos", mLookfrom);
    glBindVertexArray(mBBVaoId);
  }
}

void Renderer::endShader() {
  glUseProgram(0);
  _currentShader = nullptr;
}

void Renderer::setUniform(const std::string& name, float x, float y, float z) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name.c_str(), x, y, z);
}

void Renderer::setUniform(const std::string& name, 
    float x, float y, float z, float w) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name.c_str(), glm::vec4(x, y, z, w));
}

void Renderer::setUniform(const std::string& name, const glm::vec2 &v) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name.c_str(), v);
}

void Renderer::setUniform(const std::string& name, const glm::vec3 &v) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name.c_str(), v);
}

void Renderer::setUniform(const std::string& name, const glm::vec4 &v) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name.c_str(), v);
}

void Renderer::setUniform(const std::string& name, const glm::mat4 &m) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name.c_str(), m);
}

void Renderer::setUniform(const std::string& name, const glm::mat3 &m) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name.c_str(), m);
}

void Renderer::setUniform(const std::string& name, float val) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name.c_str(), val);
}

void Renderer::setUniform(const std::string& name, int val) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name.c_str(), val);
}

void Renderer::setUniform(const std::string& name, bool val) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name.c_str(), val);
}

void Renderer::setUniform(const std::string& name, GLuint val) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name.c_str(), val);
}

void Renderer::loadCubemap(const std::string& name, const string& dir, int slot) {
  vector<string> faces = {
      dir + "/right.png",
      dir + "/left.png",
      dir + "/top.png",
      dir + "/bottom.png",
      dir + "/back.png",
      dir + "/front.png",
  };
  loadCubemap(name, faces, slot);
}

void Renderer::loadCubemap(const std::string& name, const vector<string>& faces, int slot) {
  vector<Image> images;
  for (string filename : faces) {
      Image img;
      img.load(filename);
      images.push_back(img);
  }
  loadCubemap(name, images, slot);
}

void Renderer::loadCubemap(const std::string& name, const vector<Image>& faces, int slot) {
  glEnable(GL_TEXTURE0 + slot);
  glActiveTexture(GL_TEXTURE0 + slot);

  GLuint texId;
  if (_textures.count(name) == 0) {
    glGenTextures(1, &texId);
    Texture tex{texId, slot};
    _textures[name] = tex;

  } else {
    texId = _textures[name].texId;
  }
  glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

  GLuint targets[] = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

  for (int i = 0; i < faces.size(); i++) {
    if (faces[i].data()) {
      glTexImage2D(targets[i],
        0, GL_RGBA, faces[i].width(), faces[i].height(),
        0, GL_RGBA, GL_UNSIGNED_BYTE, faces[i].data());
    } 
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Renderer::loadTexture(const std::string& name, const std::string& fileName, int slot) {
  Image img;
  img.load(fileName);
  loadTexture(name, img, slot);
}

void Renderer::loadTexture(const std::string& name, const Image& image, int slot) {
  glEnable(GL_TEXTURE0 + slot);
  glActiveTexture(GL_TEXTURE0 + slot);

  GLuint texId;
  if (_textures.count(name) == 0) {
    glGenTextures(1, &texId);
    _textures[name] = Texture{texId, slot};
  } else {
    texId = _textures[name].texId;
  }

  glBindTexture(GL_TEXTURE_2D, texId);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, image.width(), image.height());
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width(), image.height(),
      GL_RGBA, GL_UNSIGNED_BYTE, image.data());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Renderer::loadShader(const std::string& name, 
    const std::string& vs, const std::string& fs) {

  Shader* shader = new Shader();

  std::cout << "Compiling: " << vs << std::endl;
  shader->compileShader(vs);

  std::cout << "Compiling: " << fs << std::endl;
  shader->compileShader(fs);

  shader->link(); 
  std::cout << "Loaded shader: " << name << std::endl;

  _shaders[name] = shader;
}

}  // namespace agl

