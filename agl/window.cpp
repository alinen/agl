// copyright 2020, savvy_sine, alinen

#include "agl/window.h"
#include <string>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace agl {

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

static Window* theInstance = 0;

static void error_callback(int error, const char* description) {
  fputs("\n", stderr);
  fputs(description, stderr);
  fputs("\n", stderr);
}

Window::Window() :
  _windowWidth(500),
  _windowHeight(500),
  _backgroundColor(0.0f),
  _elapsedTime(0.0),
  _cameraEnabled(true),
  _lastx(0), _lasty(0),
  _dt(-1.0) {
  init();
}

Window::~Window() {
  renderer.cleanup();
  glfwTerminate();
}

void Window::background(const vec3& color) {
  _backgroundColor = color;
  glClearColor(color[0], color[1], color[2], 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::noLoop() {
  glfwSetWindowShouldClose(_window, GL_TRUE);
}

void Window::setupOrthoScene(const vec3& center, const vec3& dim) {
  vec3 minCorner = center - 0.5f * dim;
  vec3 maxCorner = center + 0.5f * dim;
  ortho(minCorner.x, maxCorner.x,
        minCorner.y, maxCorner.y,
        minCorner.z, maxCorner.z);

  vec3 offset(0, 0.25f * dim.y, 0.25f * dim.z);
  lookAt(center - offset, center);
}

void Window::setupPerspectiveScene(const vec3& center, const vec3& dim) {
  float w = dim[0];
  float h = dim[1];
  float d = dim[2];
  float vfov = glm::radians(60.0);
  float angle = 0.5f * vfov;
  float aspect = _windowWidth / _windowHeight;
  float dist;
  vec3 up, eye, look;

  if (d > h) {
    up = vec3(0, 0.0, 1.0);
    if (w > d) {
      dist = w*0.5 / tan(angle);
    } else {
      dist = d*aspect*0.5 / tan(angle);
    }
    eye = 0.5f * vec3(-dist, dist, h);
    eye = eye + center;
    look = center;

  } else {
    up = vec3(0, 1.0, 0.0);
    if (w > h) {
      dist = w*0.5 / tan(angle);
    } else {
      dist = h*aspect*0.5 / tan(angle);
    }
    eye = 0.5f * vec3(-dist, h, dist);
    eye = center + eye;
    look = center;
  }
  float far = std::max(std::max(w, h), d) * 2;
  perspective(vfov, aspect, far * 0.01f, far);
  lookAt(eye, look, up);
}

void Window::ortho(float minx, float maxx,
    float miny, float maxy, float minz, float maxz) {
  renderer.ortho(minx, maxx, miny, maxy, minz, maxz);
}

void Window::perspective(float fovRadians, float aspect,
    float near, float far) {
  renderer.perspective(fovRadians, aspect, near, far);
}

void Window::lookAt(const glm::vec3& camPos,
    const glm::vec3& camLook, const glm::vec3& up) {
  renderer.lookAt(camPos, camLook, up);
  camera.set(camPos, camLook, up);
}

void Window::run() {
  if (!_window) return;  // window wasn't initialized

  setup();

  while (!glfwWindowShouldClose(_window)) {
    float time = glfwGetTime();
    _dt = time - _elapsedTime;
    _elapsedTime = time;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (cameraEnabled()) {
      renderer.lookAt(camera.position(), camera.look(), camera.up());
    }
    renderer.identity();
    draw();  // user function
    renderer.cleanupShaders();

    glfwSwapBuffers(_window);
    glfwPollEvents();
  }
}

bool Window::screenshot(const std::string& filename) {
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  int x = viewport[0];
  int y = viewport[1];
  int width = viewport[2];
  int height = viewport[3];

  size_t size = static_cast<size_t>(width * height * 4);
  unsigned char *data = static_cast<unsigned char*>(malloc(size));

  if (!data) {
    return false;
  }

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

  // Using Image introduces another copy but makes it easier to keep all
  // stb code referenced from a single file (needed for header-only include)
  Image image;
  image.set(width, height, data);
  bool result = image.save(filename);

  free(data);
  return result;
}

float Window::height() const {
  return static_cast<float>(_windowHeight);
}

float Window::width() const {
  return static_cast<float>(_windowWidth);
}

float Window::dt() const {
  return _dt;
}

float Window::elapsedTime() const {
  return _elapsedTime;
}

glm::vec2 Window::mousePosition() const {
  double xpos, ypos;
  glfwGetCursorPos(_window, &xpos, &ypos);
  return glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
}

bool Window::keyIsDown(int key) const {
  int state = glfwGetKey(_window, key);
  return (state == GLFW_PRESS);
}

bool Window::mouseIsDown(int button) const {
  int state = glfwGetMouseButton(_window, button);
  return (state == GLFW_PRESS);
}

void Window::setWindowSize(int w, int h) {
  if (_windowWidth == w && _windowHeight == h) return;
  _windowWidth = w;
  _windowHeight = h;
  glfwSetWindowSize(_window, w, h);  
}

void Window::init() {
  theInstance = this;
  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
    fprintf(stderr, "ERROR: Cannot initialize GLFW\n");
    return;
  }

  // Set the GLFW window creation hints - these are optional
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_SAMPLES, 4);  // Request 4x antialiasing
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  _window = glfwCreateWindow(_windowWidth, _windowHeight, "AGL Window", 0, 0);
  if (!_window) {
    fprintf(stderr, "ERROR: Cannot initialize GLFW window\n");
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(_window);
  glfwSetKeyCallback(_window, Window::onKeyboardCb);
  glfwSetFramebufferSizeCallback(_window, Window::onResizeCb);
  glfwSetMouseButtonCallback(_window, Window::onMouseButtonCb);
  glfwSetCursorPosCallback(_window, Window::onMouseMotionCb);
  glfwSetScrollCallback(_window, Window::onScrollCb);

#ifndef APPLE
  if (glewInit() != GLEW_OK)
  {
     std::cout << "Cannot initialize GLEW\n";
     return;
  }
#endif

  // Initialize openGL and set default values
  renderer.init();
  camera.set(vec3(0.0, 0.0, 2.0), vec3(0.0));
  background(vec3(0));
}

void Window::onMouseMotionCb(GLFWwindow* win, double pX, double pY) {
  theInstance->onMouseMotion(static_cast<int>(pX), static_cast<int>(pY));
}

void Window::onMouseMotion(int pX, int pY) {
  if (cameraEnabled()) {
    camera.onMouseMotion(pX, pY);
  }

  glm::vec2 mousePos = mousePosition();
  int dx = mousePos.x - _lastx;
  int dy = mousePos.y - _lasty;
  mouseMotion(pX, pY, dx, dy);  // user hook
}

void Window::onMouseButtonCb(GLFWwindow* win,
    int button, int action, int mods) {
  theInstance->onMouseButton(button, action, mods);
}

void Window::onMouseButton(int button, int action, int mods) {
  double xpos, ypos;
  glfwGetCursorPos(_window, &xpos, &ypos);

  // ASN TODO: Save/pass modifiers so users can get it
  if (cameraEnabled()) {
    camera.onMouseButton(button, action, xpos, ypos, mods);
  }

  if (action == GLFW_PRESS) {
    _lastx = xpos;
    _lasty = ypos;
    mouseDown(button, mods);

  } else if (action == GLFW_RELEASE) {
    mouseUp(button, mods);
  }

  onMouseMotion(static_cast<float>(xpos), static_cast<float>(ypos));
}

void Window::onKeyboardCb(GLFWwindow* w,
    int key, int scancode, int action, int mods) {
  theInstance->onKeyboard(key, scancode, action, mods);
}

void Window::onKeyboard(int key, int scancode, int action, int mods) {
  // Exit on ESC key.
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(_window, GL_TRUE);
  }

  if (cameraEnabled()) {
    camera.onKeyboard(key, scancode, action, mods);
  }

  if (action == GLFW_PRESS) {
    keyDown(key, mods);
  } else if (action == GLFW_RELEASE) {
    keyUp(key, mods);
  }
}

void Window::onScrollCb(GLFWwindow* win, double xoffset, double yoffset) {
  theInstance->onScroll(
    static_cast<float>(xoffset),
    static_cast<float>(yoffset));
}

void Window::onScroll(float xoffset, float yoffset) {
  if (cameraEnabled()) {
    camera.onScroll(xoffset, yoffset);
  }
  scroll(xoffset, yoffset);  // user hook
}

void Window::onResizeCb(GLFWwindow* window, int width, int height) {
  theInstance->onResize(width, height);
}

void Window::onResize(int width, int height) {
  _windowWidth = width;
  _windowHeight = height;
  glViewport(0, 0, width, height);
  resize(width, height);  // user function
}

}  // namespace agl
