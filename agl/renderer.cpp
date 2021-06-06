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
  mSkybox = 0;
  _sphere = 0;
  _initialized = false;
}

Renderer::~Renderer() {
  cleanup();
}

void Renderer::cleanup() {
  delete _sphere;
  _sphere = 0;

  delete mSkybox;
  mSkybox = 0;

  for (auto it : _shaders) {
    delete it.second; 
  }
  _shaders.clear();
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
  mSkybox = new SkyBox(10);
  vector<string> faces = {
    "../textures/sky/right.png",
    "../textures/sky/left.png",
    "../textures/sky/top.png",
    "../textures/sky/bottom.png",
    "../textures/sky/back.png",
    "../textures/sky/front.png",
  };
  mCubemap = loadCubemap(faces);
  loadShader("cubemap", "../shaders/cubemap.vs", "../shaders/cubemap.fs");
}

void Renderer::initMesh() {
  loadShader("phong", "../shaders/phong.vs", "../shaders/phong.fs");

  // Set default parameters
  beginShader("phong");
  setUniform("uGamma", 0.8f);
  setUniform("uMaterial.Ks", 1.0f, 1.0f, 1.0f);
  setUniform("uMaterial.Kd", 0.4f, 0.6f, 1.0f);
  setUniform("uMaterial.Ka", 0.1f, 0.1f, 0.1f);
  setUniform("uMaterial.shininess", 80.0f);
  setUniform("uLight.position", 100.0f, 100.0f, 100.0f, 1.0f);
  setUniform("uLight.color", 1.0f, 1.0f, 1.0f);
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

  loadShader("billboard", 
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

void Renderer::begin(GLuint texIf, BlendMode mode) {
  assert(_initialized);

  blendMode(mode);
  beginShader("billboard");

  mat4 mvp = mProjectionMatrix * mViewMatrix;
  setUniform("MVP", mvp); 
  setUniform("CameraPos", mLookfrom);

  glBindTexture(GL_TEXTURE_2D, texIf);
  setUniform("image", 0);

  glBindVertexArray(mBBVaoId);
  glEnableVertexAttribArray(0);  // 0 -> Send VertexPositions to array #0
}

void Renderer::quad(const glm::vec3& pos, const glm::vec4& color, float size) {
  assert(_initialized);

  setUniform("Offset", pos);
  setUniform("Color", color);
  setUniform("Size", size);

  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::end() {
  assert(_initialized);
  endShader();
}

void Renderer::skybox() {
  assert(_initialized);
  blendMode(DEFAULT);
  beginShader("cubemap");

  glBindTexture(GL_TEXTURE_CUBE_MAP, mCubemap);
  setUniform("cubemap", 1);

  mat4 mvp = mProjectionMatrix * mViewMatrix;
  setUniform("uVP", mvp);

  mSkybox->render();
  endShader();
}

void Renderer::sphere() {
  mesh(mat4(1), *_sphere);
}

void Renderer::mesh(const mat4& trs, const TriangleMesh& mesh) {
  assert(_initialized);

  blendMode(DEFAULT);
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
}

void Renderer::endShader() {
  glUseProgram(0);
  _currentShader = nullptr;
}

void Renderer::setUniform(const char *name, float x, float y, float z) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name, x, y, z);
}

void Renderer::setUniform(const char *name, 
    float x, float y, float z, float w) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name, glm::vec4(x, y, z, w));
}

void Renderer::setUniform(const char *name, const glm::vec2 &v) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name, v);
}

void Renderer::setUniform(const char *name, const glm::vec3 &v) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name, v);
}

void Renderer::setUniform(const char *name, const glm::vec4 &v) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name, v);
}

void Renderer::setUniform(const char *name, const glm::mat4 &m) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name, m);
}

void Renderer::setUniform(const char *name, const glm::mat3 &m) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name, m);
}

void Renderer::setUniform(const char *name, float val) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name, val);
}

void Renderer::setUniform(const char *name, int val) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name, val);
}

void Renderer::setUniform(const char *name, bool val) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name, val);
}

void Renderer::setUniform(const char *name, GLuint val) {
  assert(_currentShader != nullptr);
  _currentShader->setUniform(name, val);
}

GLuint Renderer::loadCubemap(const vector<string>& cubeFaces) {
  glEnable(GL_TEXTURE1);
  glActiveTexture(GL_TEXTURE1);
  GLuint texBackId;
  glGenTextures(1, &texBackId);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texBackId);

  GLuint targets[] = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

  for (int i = 0; i < cubeFaces.size(); i++) {
    Image image;
    image.load(cubeFaces[i]);

    if (image.data()) {
      glTexImage2D(targets[i],
        0, GL_RGBA, image.width(), image.height(),
        0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    } else {
      std::cout << "Image not loaded " << cubeFaces[i] << std::endl;
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return texBackId;
}


GLuint Renderer::loadTexture(const std::string& filename) {
  Image image;
  image.load(filename);

  glEnable(GL_TEXTURE0);
  glActiveTexture(GL_TEXTURE0);

  GLuint texId;
  glGenTextures(1, &texId);
  glBindTexture(GL_TEXTURE_2D, texId);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, image.width(), image.height());
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width(), image.height(),
      GL_RGBA, GL_UNSIGNED_BYTE, image.data());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  return texId;
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

