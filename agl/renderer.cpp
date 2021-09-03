// Copyright 2020, Savvy Sine, Aline Normoyle

#include "agl/renderer.h"
#include <fstream>
#include <sstream>
#include "agl/image.h"
#include "agl/shader.h"
#include "agl/mesh/sphere.h"
#include "agl/mesh/cube.h"
#include "agl/mesh/cylinder.h"
#include "agl/mesh/capsule.h"
#include "agl/mesh/teapot.h"
#include "agl/mesh/torus.h"
#include "agl/mesh/plane.h"
#include "agl/mesh/skybox.h"
#define FONTSTASH_IMPLEMENTATION
#include "fontstash/fontstash.h"
#define GLFONTSTASH_IMPLEMENTATION
#include "fontstash/gl3corefontstash.h"

namespace agl {

static FONScontext* _fs = NULL;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;
using std::string;
using std::vector;

Renderer::Renderer() {
  _cube = 0;
  _cone = 0;
  _capsule = 0;
  _cylinder = 0;
  _teapot = 0;
  _torus = 0;
  _plane = 0;
  _sphere = 0;
  _skybox = 0;
  _blendMode = DEFAULT;

  _fontNormal = FONS_INVALID;
  _fs = NULL;

  _currentShader = 0;
  _initialized = false;
}

Renderer::~Renderer() {
  cleanup();
}

void Renderer::cleanup() {
  glfonsDelete(_fs);
  _fs = NULL;
  _fontNormal = FONS_INVALID;

  delete _cube;
  delete _cone;
  delete _capsule;
  delete _cylinder;
  delete _teapot;
  delete _torus;
  delete _plane;
  delete _sphere;
  delete _skybox;

  _cube = 0;
  _cone = 0;
  _capsule = 0;
  _cylinder = 0;
  _teapot = 0;
  _torus = 0;
  _plane = 0;
  _sphere = 0;
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
  return _lookfrom;
}

void Renderer::init() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // setup default camera and projection
  float halfw = 1.0;
  float halfh = 1.0;
  ortho(-halfw, halfw, -halfh, halfh, -10.0f, 10.0f);
  lookAt(vec3(0, 0, 2), vec3(0, 0, 0));

  initLines();
  initBillboards();
  initMesh();
  initText();
  loadShader("cubemap", "../shaders/cubemap.vs", "../shaders/cubemap.fs");
  loadShader("unlit", "../shaders/unlit.vs", "../shaders/unlit.fs");
  loadShader("normals", "../shaders/normals.vs", "../shaders/normals.fs");

  _cube = new Cube(1.0f);
  _cone = new Cylinder(0.5f, 0.01, 1, 40);
  _capsule = new Capsule(0.25, 0.5, 40, 40);
  _cylinder = new Cylinder(0.5, 1.0, 40.0);
  _teapot = new Teapot(13, mat4(1.0));
  _torus = new Torus(0.5, 0.25, 40, 40);
  _plane = new Plane(1.0, 1.0, 1.0, 1.0);
  _sphere = new Sphere(0.5f, 40, 40);
  _skybox = new SkyBox(1);
  _trs = mat4(1.0);
  _initialized = true;

  beginShader("phong");  // phong is default
}

void Renderer::initMesh() {
  loadShader("phong", "../shaders/phong.vs", "../shaders/phong.fs");

  // Set default parameters
  beginShader("phong");
  setUniform("Gamma", 0.8f);
  setUniform("Material.specular", 1.0f, 1.0f, 1.0f);
  setUniform("Material.diffuse", 1.0f, 1.0f, 1.0f);
  setUniform("Material.ambient", 0.1f, 0.1f, 0.1f);
  setUniform("Material.shininess", 80.0f);
  setUniform("Light.position", 100.0f, 100.0f, 100.0f, 1.0f);
  setUniform("Light.color", 1.0f, 1.0f, 1.0f);
  setUniform("Fog.enabled", false);
  setUniform("HasUV", false);

  setUniform("MainTexture.enabled", false);
  setUniform("MainTexture.offset", vec2(0.0f));
  setUniform("MainTexture.tile", vec2(1.0f));

  setUniform("DetailTexture.enabled", false);
  setUniform("DetailTexture.offset", vec2(0.0f));
  setUniform("DetailTexture.tile", vec2(1.0f));
  endShader();
}

void Renderer::initLines() {
  loadShader("lines", "../shaders/lines.vs", "../shaders/lines.fs");
  const float positions[] = {
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f
  };

  glGenBuffers(1, &mVboLinePosId);
  glBindBuffer(GL_ARRAY_BUFFER, mVboLinePosId);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_DYNAMIC_DRAW);

  glGenBuffers(1, &mVboLineColorId);
  glBindBuffer(GL_ARRAY_BUFFER, mVboLineColorId);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_DYNAMIC_DRAW);

  glGenVertexArrays(1, &mVaoLineId);
  glBindVertexArray(mVaoLineId);

  glEnableVertexAttribArray(0);  // 0 -> VertexPositions to array #0
  glBindBuffer(GL_ARRAY_BUFFER, mVboLinePosId);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<GLubyte*>(0));

  glEnableVertexAttribArray(1);  // 1 -> VertexPositions to array #1
  glBindBuffer(GL_ARRAY_BUFFER, mVboLineColorId);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, static_cast<GLubyte*>(0));
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

void Renderer::initText() {
    loadShader("text", "../shaders/text.vs", "../shaders/text.fs");
    _fs = glfonsCreate(512, 512, FONS_ZERO_TOPLEFT);
    if (_fs == NULL) {
      printf("Could not create stash.\n");
    }

    _fontNormal = fonsAddFont(_fs, "sans", "../fonts/DroidSerif-Regular.ttf");
    if (_fontNormal == FONS_INVALID) {
      printf("Could not add font normal.\n");
    }

    _fontColor = glfonsRGBA(255, 255, 255, 255);
    _fontSize = 20.0;
}

void Renderer::blendMode(BlendMode mode) {
  if (mode == ADD) {
    _blendMode = ADD;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);  // Additive blend

  } else if (mode == BLEND) {
    _blendMode = BLEND;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Alpha blend

  } else {
    _blendMode = DEFAULT;
    glDisable(GL_BLEND);
  }
}

void Renderer::perspective(float fovRadians,
    float aspect, float near, float far) {
  _projectionMatrix = glm::perspective(fovRadians, aspect, near, far);
}

void Renderer::ortho(float minx, float maxx,
    float miny, float maxy, float minz, float maxz) {
  _projectionMatrix = glm::ortho(minx, maxx, miny, maxy, minz, maxz);
}

void Renderer::lookAt(const vec3& lookfrom,
    const vec3& lookat, const vec3& up) {
  _lookfrom = lookfrom;
  _viewMatrix = glm::lookAt(lookfrom, lookat, up);
}

void Renderer::texture(const std::string& uniformName,
    const std::string& textureName) {
  assert(_textures.count(textureName) != 0);

  glActiveTexture(GL_TEXTURE0 + _textures[textureName].slot);
  glBindTexture(GL_TEXTURE_2D, _textures[textureName].texId);
  setUniform(uniformName, _textures[textureName].slot);
}

void Renderer::fontColor(const glm::vec4& c) {
  unsigned char r = (unsigned char) (c[0]*255.9);
  unsigned char g = (unsigned char) (c[1]*255.9);
  unsigned char b = (unsigned char) (c[2]*255.9);
  unsigned char a = (unsigned char) (c[3]*255.9);
  _fontColor = glfonsRGBA(r, g, b, a);
}

void Renderer::fontSize(int s) {
  _fontSize = s;
}

float Renderer::textWidth(const std::string& s) {
  float w = fonsTextBounds(_fs, 0, 0, s.c_str(), NULL, NULL);
  return w;
}

float Renderer::textHeight() {
  float lineh = 0;
  fonsVertMetrics(_fs, NULL, NULL, &lineh);
  return lineh;
}

void Renderer::text(const std::string& text, float x, float y) {
  float viewport[4]; 
  glGetFloatv(GL_VIEWPORT, viewport);

  mat4 ortho = glm::ortho(0.0f, viewport[2], viewport[3], 0.0f, -100.0f, 100.0f);
  BlendMode m = _blendMode;
  blendMode(BLEND);
  beginShader("text");
  setUniform("MVP", ortho);

  fonsSetSize(_fs, _fontSize);
  fonsSetFont(_fs, _fontNormal);
  fonsSetColor(_fs, _fontColor);
  fonsDrawText(_fs, x, y, text.c_str(), NULL);
  //std::cout << viewport[2] << " " << viewport[3] << std::endl;

  endShader();
  blendMode(m);
}

void Renderer::line(const glm::vec3& p1, const glm::vec3& p2,
    const glm::vec3& c1, const glm::vec3& c2) {
  assert(_initialized);

  mat4 mvp = _projectionMatrix * _viewMatrix;
  setUniform("MVP", mvp);

  GLfloat positions[6];
  positions[0] = p1.x;
  positions[1] = p1.y;
  positions[2] = p1.z;
  positions[3] = p2.x;
  positions[4] = p2.y;
  positions[5] = p2.z;

  GLfloat colors[6];
  colors[0] = c1.x;
  colors[1] = c1.y;
  colors[2] = c1.z;
  colors[3] = c2.x;
  colors[4] = c2.y;
  colors[5] = c2.z;

  glBindVertexArray(mVaoLineId);
  glBindBuffer(GL_ARRAY_BUFFER, mVboLinePosId);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, mVboLineColorId);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), colors, GL_DYNAMIC_DRAW);

  glDrawArrays(GL_LINES, 0, 2);
}

void Renderer::sprite(const glm::vec3& pos,
    const glm::vec4& color, float size) {
  assert(_initialized);

  mat4 mvp = _projectionMatrix * _viewMatrix;
  setUniform("MVP", mvp);
  setUniform("CameraPos", _lookfrom);
  setUniform("Offset", pos);
  setUniform("Color", color);
  setUniform("Size", size);

  glBindVertexArray(mBBVaoId);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::cubemap(const std::string& uniformName,
    const std::string& textureName) {
  assert(_textures.count(textureName) != 0);

  glBindTexture(GL_TEXTURE_CUBE_MAP, _textures[textureName].texId);
  setUniform(uniformName, _textures[textureName].slot);
}

void Renderer::skybox(float size) {
  assert(_initialized);

  mat4 s = glm::scale(mat4(1.0f), vec3(size));
  mat4 mvp = _projectionMatrix * _viewMatrix * s;
  setUniform("MVP", mvp);
  _skybox->render();
}

void Renderer::push() {
  _stack.push_front(_trs);
}

void Renderer::pop() {
  if (_stack.size() == 0) return;
  _trs = _stack.front();
  _stack.pop_front();
}

void Renderer::identity() {
  _trs = mat4(1.0);
}

void Renderer::scale(const vec3& xyz) {
  _trs = _trs * glm::scale(mat4(1.0), xyz);
}

void Renderer::translate(const vec3& xyz) {
  _trs = _trs * glm::translate(mat4(1.0), xyz);
}

void Renderer::rotate(float angleRad, const vec3& axis) {
  _trs = _trs * glm::rotate(mat4(1.0), angleRad, axis);
}

void Renderer::transform(const glm::mat4& trs) {
  _trs = _trs * trs;
}

void Renderer::teapot() {
  mesh(*_teapot);
}

void Renderer::plane() {
  mesh(*_plane);
}

void Renderer::cylinder() {
  mesh(*_cylinder);
}

void Renderer::capsule() {
  mesh(*_capsule);
}

void Renderer::torus() {
  mesh(*_torus);
}

void Renderer::cone() {
  mesh(*_cone);
}

void Renderer::cube() {
  mesh(*_cube);
}

void Renderer::sphere() {
  mesh(*_sphere);
}

void Renderer::mesh(const Mesh& mesh) {
  assert(_initialized);

  mat4 mv = _viewMatrix * _trs;
  mat4 mvp = _projectionMatrix * mv;
  mat3 nmv = transpose(inverse(mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]))));

  setUniform("MVP", mvp);
  setUniform("ModelViewMatrix", mv);
  setUniform("NormalMatrix", nmv);
  setUniform("ModelMatrix", _trs);
  setUniform("HasUV", mesh.hasUV());

  mesh.render();
}

void Renderer::cleanupShaders() {
  while (_shaderStack.size() > 1) {
    endShader();
  }
}

void Renderer::beginShader(const std::string& shaderName) {
  assert(_shaders.count(shaderName) != 0);

  _shaderStack.push_front(_currentShader);
  _currentShader = _shaders[shaderName];
  _currentShader->use();
}

void Renderer::endShader() {
  assert(_shaderStack.size() > 0);

  _currentShader = _shaderStack.front();
  _shaderStack.pop_front();

  if (_currentShader != nullptr) {
    _currentShader->use();

  } else {
    glUseProgram(0);
  }
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

void Renderer::loadCubemap(const std::string& name,
    const string& dir, int slot) {
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

void Renderer::loadCubemap(const std::string& name,
    const vector<string>& faces, int slot) {
  vector<Image> images;
  for (string filename : faces) {
      Image img;
      img.load(filename);
      images.push_back(img);
  }
  loadCubemap(name, images, slot);
}

void Renderer::loadCubemap(const std::string& name,
    const vector<Image>& faces, int slot) {
  if (slot == GLFONS_FONT_TEXTURE_SLOT) {
    std::cout << "WARNING: slot " << slot << " conflicts with font texture\n";
  }
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

void Renderer::loadTexture(const std::string& name,
    const std::string& fileName, int slot) {
  Image img;
  img.load(fileName);
  loadTexture(name, img, slot);
}

void Renderer::loadTexture(const std::string& name,
    const Image& image, int slot) {
  if (slot == GLFONS_FONT_TEXTURE_SLOT) {
    std::cout << "WARNING: slot " << slot << " conflicts with font texture\n";
  }
  glEnable(GL_TEXTURE0 + slot);
  glActiveTexture(GL_TEXTURE0 + slot);

  GLuint texId;
  if (_textures.count(name) == 0) {
    glGenTextures(1, &texId);
    _textures[name] = Texture{texId, slot};
  } else {
    std::cout << "Warning: texture already registered with name: " << 
        name << std::endl;
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

void Renderer::beginRenderTexture(const std::string& targetName) {
  assert(_renderTextures.count(targetName) != 0);
  assert(_activeRenderTexture == "");

  RenderTexture& tex = _renderTextures[targetName];
  glBindFramebuffer(GL_FRAMEBUFFER, tex.handleId);

  // Cache viewport size so it can be restored later
  glGetIntegerv(GL_VIEWPORT, tex.winProps);
  glViewport(0, 0, tex.width, tex.height);
  _activeRenderTexture = targetName;
}

void Renderer::endRenderTexture() {
  assert(_activeRenderTexture.size() != 0);
  glFlush();

  // unbind fbo and revert to default (the screen)
  RenderTexture target = _renderTextures[_activeRenderTexture];
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(target.winProps[0],
             target.winProps[1],
             target.winProps[2],
             target.winProps[3]);

  _activeRenderTexture = "";
}

void Renderer::loadRenderTexture(const std::string& name,
    int slot, int width, int height) {
  if (slot == GLFONS_FONT_TEXTURE_SLOT) {
    std::cout << "WARNING: slot " << slot << " conflicts with font texture\n";
  }
  // Generate and bind the framebuffer
  GLuint fboHandle;
  glGenFramebuffers(1, &fboHandle);
  glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

  // Create the texture object
  GLuint renderTex;
  glGenTextures(1, &renderTex);
  glActiveTexture(GL_TEXTURE0 + slot);  // put in given slot!!
  glBindTexture(GL_TEXTURE_2D, renderTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
      GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // save texture as an available texture object with the same name
  _textures[name] = Texture{renderTex, slot};

  // Bind the texture to the FBO
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
      GL_TEXTURE_2D, renderTex, 0);

  // Create the depth buffer
  GLuint depthBuf;
  glGenRenderbuffers(1, &depthBuf);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

  // Bind the depth buffer to the FBO
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, depthBuf);

  // Set the targets for the fragment output variables
  GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, drawBuffers);

  GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if( result != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "Framebuffer error: " << result << std::endl;
  }

  RenderTexture target;
  target.handleId = fboHandle;
  target.textureId = renderTex;
  target.depthId = depthBuf;
  target.slot = slot;
  target.width = width;
  target.height = height;
  _renderTextures[name] = target;

  // unbind fbo and revert to default (the screen)
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


}  // namespace agl

