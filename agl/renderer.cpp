// Copyright 2020, Savvy Sine, aline

#include "agl/renderer.h"
#include <fstream>
#include <sstream>
#include "agl/sphere.h"

namespace agl {
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;
using std::string;
using std::vector;

static void PrintShaderErrors(GLuint id, const std::string label) {
  std::cerr << label << " failed\n";
  GLint logLen;
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);
  if (logLen > 0) {
    char* log = static_cast<char*>(malloc(logLen));
    GLsizei written;
    glGetShaderInfoLog(id, logLen, &written, log);
    std::cerr << "Shader log: " << log << std::endl;
    free(log);
  }
}

Renderer::Renderer() {
  mSkybox = 0;
  initialized_ = false;
}

Renderer::~Renderer() {
  cleanup();
}

void Renderer::cleanup() {
  delete mSkybox;
  mSkybox = 0;
  initialized_ = false;
}

bool Renderer::initialized() const {
  return initialized_;
}


vec3 Renderer::cameraPosition() const {
  return mLookfrom;
}

void Renderer::init() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  float halfw = 1.0;
  float halfh = 1.0;
  ortho(-halfw, halfw, -halfh, halfh, -2.0f, 2.0f);
  lookAt(vec3(0, 0, 2), vec3(0, 0, 0));

  initBillboards();
  initCubemap();
  initMesh();
  initialized_ = true;
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
  mCMShaderId = loadShader("../shaders/cubemap.vs", "../shaders/cubemap.fs");
}

void Renderer::initMesh() {
  mMShaderId = loadShader("../shaders/phong.vs", "../shaders/phong.fs");
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

  mBBShaderId = loadShader(
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
  assert(initialized_);

  glUseProgram(mBBShaderId);
  blendMode(mode);

  mat4 mvp = mProjectionMatrix * mViewMatrix;
  glUniformMatrix4fv(glGetUniformLocation(mBBShaderId, "uVP"),
      1, GL_FALSE, &mvp[0][0]);
  glUniform3f(glGetUniformLocation(mBBShaderId, "uCameraPos"),
      mLookfrom[0], mLookfrom[1], mLookfrom[2]);

  glBindTexture(GL_TEXTURE_2D, texIf);
  GLuint locId = glGetUniformLocation(mBBShaderId, "image");
  glUniform1i(locId, 0);

  glBindVertexArray(mBBVaoId);
  glEnableVertexAttribArray(0);  // 0 -> Send VertexPositions to array #0
}

void Renderer::quad(const glm::vec3& pos, const glm::vec4& color, float size) {
  assert(initialized_);
  glUniform3f(glGetUniformLocation(mBBShaderId, "uOffset"),
      pos[0], pos[1], pos[2]);
  glUniform4f(glGetUniformLocation(mBBShaderId, "uColor"),
      color[0], color[1], color[2], color[3]);
  glUniform1f(glGetUniformLocation(mBBShaderId, "uSize"), size);

  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::end() {
  assert(initialized_);
  glUseProgram(0);
}

void Renderer::skybox() {
  assert(initialized_);
  blendMode(DEFAULT);
  glUseProgram(mCMShaderId);

  glBindTexture(GL_TEXTURE_CUBE_MAP, mCubemap);
  GLuint locId = glGetUniformLocation(mCMShaderId, "cubemap");
  glUniform1i(locId, 1);

  mat4 mvp = mProjectionMatrix * mViewMatrix;
  glUniformMatrix4fv(glGetUniformLocation(mCMShaderId, "uVP"),
      1, GL_FALSE, &mvp[0][0]);

  mSkybox->render();
}

void Renderer::mesh(const mat4& trs, const TriangleMesh& mesh) {
  assert(initialized_);
  blendMode(DEFAULT);
  glUseProgram(mMShaderId);

  // GLuint timeParamId = glGetUniformLocation(mMShaderId, "time");
  GLuint mvpId = glGetUniformLocation(mMShaderId, "uMVP");
  GLuint mvId = glGetUniformLocation(mMShaderId, "uMV");
  GLuint nmvId = glGetUniformLocation(mMShaderId, "uNMV");

  glUniform1f(glGetUniformLocation(mMShaderId, "uGamma"), 0.8f);
  glUniform3f(glGetUniformLocation(mMShaderId, "uMaterial.Ks"), 1.0, 1.0, 1.0);
  glUniform3f(glGetUniformLocation(mMShaderId, "uMaterial.Kd"), 0.4, 0.6, 1.0);
  glUniform3f(glGetUniformLocation(mMShaderId, "uMaterial.Ka"), 0.1, 0.1, 0.1);
  glUniform1f(glGetUniformLocation(mMShaderId, "uMaterial.shininess"), 80.0);
  glUniform4f(glGetUniformLocation(mMShaderId, "uLight.position"),
      100.0, 100.0, 100.0, 1.0);
  glUniform3f(glGetUniformLocation(mMShaderId, "uLight.color"), 1.0, 1.0, 1.0);

  mat4 mv = mViewMatrix * trs;
  mat4 mvp = mProjectionMatrix * mv;
  mat3 nmv = mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]));
  glUniformMatrix3fv(nmvId, 1, GL_FALSE, &nmv[0][0]);
  glUniformMatrix4fv(mvId, 1, GL_FALSE, &mv[0][0]);
  glUniformMatrix4fv(mvpId, 1, GL_FALSE, &mvp[0][0]);

  mesh.render();
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

std::string Renderer::loadShaderFromFile(const std::string& fileName) {
  std::ifstream file(fileName);
  if (!file) {
    std::cout << "Cannot load file: " << fileName << std::endl;
    return "";
  }

  std::stringstream code;
  code << file.rdbuf();
  file.close();

  return code.str();
}

GLuint Renderer::loadShader(
    const std::string& vertex, const std::string& fragment) {

  GLint result;
  std::string vertexShader = loadShaderFromFile(vertex);
  const char* vertexShaderRaw = vertexShader.c_str();
  GLuint vshaderId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vshaderId, 1, &vertexShaderRaw, NULL);
  glCompileShader(vshaderId);
  glGetShaderiv(vshaderId, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    PrintShaderErrors(vshaderId, "Vertex shader: "+vertex);
    return -1;
  }

  std::string fragmentShader = loadShaderFromFile(fragment);
  const char* fragmentShaderRaw = fragmentShader.c_str();
  GLuint fshaderId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fshaderId, 1, &fragmentShaderRaw, NULL);
  glCompileShader(fshaderId);
  glGetShaderiv(fshaderId, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    PrintShaderErrors(fshaderId, "Fragment shader: "+fragment);
    return -1;
  }

  GLuint shaderId = glCreateProgram();
  glAttachShader(shaderId, vshaderId);
  glAttachShader(shaderId, fshaderId);
  glLinkProgram(shaderId);
  glGetShaderiv(shaderId, GL_LINK_STATUS, &result);
  if (result == GL_FALSE) {
    PrintShaderErrors(shaderId, "Shader link");
    return -1;
  }
  return shaderId;
}
}  // namespace agl

