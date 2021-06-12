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
  windowWidth_(500),
  windowHeight_(500),
  elapsedTime_(0.0),
  dt_(-1.0) {
  init();
}

Window::~Window() {
  renderer.cleanup();
  glfwTerminate();
}

void Window::background(const vec3& color) {
  glClearColor(color[0], color[1], color[2], 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::run() {
  if (!window_) return;  // window wasn't initialized

  background(vec3(0));
  setup();

  while (!glfwWindowShouldClose(window_)) {
    float time = glfwGetTime();
    dt_ = time - elapsedTime_;
    elapsedTime_ = time;

    background(vec3(0));
    renderer.lookAt(_camera.position(), _camera.look(), _camera.up());
    renderer.identity();

    draw();  // user function

    glfwSwapBuffers(window_);
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
  return static_cast<float>(windowHeight_);
}

float Window::width() const {
  return static_cast<float>(windowWidth_);
}

float Window::dt() const {
  return dt_;
}

float Window::elapsedTime() const {
  return elapsedTime_;
}

int Window::mouseX() const {
  double xpos, ypos;
  glfwGetCursorPos(window_, &xpos, &ypos);
  return xpos;
}

int Window::mouseY() const {
  double xpos, ypos;
  glfwGetCursorPos(window_, &xpos, &ypos);
  return ypos;
}

bool Window::keyIsDown(int key) const {
  int state = glfwGetKey(window_, key);
  return (state == GLFW_PRESS);
}

bool Window::mouseIsDown(int button) const {
  int state = glfwGetMouseButton(window_, button);
  return (state == GLFW_PRESS);
}

void Window::setSize(int w, int h) {
  if (windowWidth_ == w && windowHeight_ == h) return;
  glfwSetWindowSize(window_, w, h);
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

  window_ = glfwCreateWindow(windowWidth_, windowHeight_, "AGL Window", 0, 0);
  if (!window_) {
    fprintf(stderr, "ERROR: Cannot initialize GLFW window\n");
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(window_);
  glfwSetKeyCallback(window_, Window::onKeyboardCb);
  glfwSetFramebufferSizeCallback(window_, Window::onResizeCb);
  glfwSetMouseButtonCallback(window_, Window::onMouseButtonCb);
  glfwSetCursorPosCallback(window_, Window::onMouseMotionCb);
  glfwSetScrollCallback(window_, Window::onScrollCb);

  // check that os doesn't override window size
  int width, height;
  glfwGetFramebufferSize(window_, &width, &height);
  onResize(width, height);

  // Initialize openGL
  renderer.init();

  // ASN TODO: Init based on volume size
  _camera.set(vec3(0.0, 0.0, 2.0), vec3(0.0));
}

void Window::onMouseMotionCb(GLFWwindow* win, double pX, double pY) {
  theInstance->onMouseMotion(static_cast<int>(pX), static_cast<int>(pY));
}

void Window::onMouseMotion(int pX, int pY) {
  _camera.onMouseMotion(pX, pY);
  mouseMotion(pX, pY);  // user hook
}

void Window::onMouseButtonCb(GLFWwindow* win,
    int button, int action, int mods) {
  theInstance->onMouseButton(button, action, mods);
}

void Window::onMouseButton(int button, int action, int mods) {
  _camera.onMouseButton(button, action, mouseX(), mouseY());

  if (action == GLFW_PRESS) {
    mousePress(button, mods);
  } else if (action == GLFW_RELEASE) {
    mouseRelease(button, mods);
  }

  double xpos, ypos;
  glfwGetCursorPos(window_, &xpos, &ypos);
  onMouseMotion(static_cast<float>(xpos), static_cast<float>(ypos));
}

void Window::onKeyboardCb(GLFWwindow* w,
    int key, int scancode, int action, int mods) {
  theInstance->onKeyboard(key, scancode, action, mods);
}

void Window::onKeyboard(int key, int scancode, int action, int mods) {
  // Exit on ESC key.
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window_, GL_TRUE);
  }

  _camera.onKeyboard(key, scancode, action, mods);
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
  _camera.onScroll(xoffset, yoffset);
  scroll(xoffset, yoffset);  // user hook
}

void Window::onResizeCb(GLFWwindow* window, int width, int height) {
  theInstance->onResize(width, height);
}

void Window::onResize(int width, int height) {
  windowWidth_ = width;
  windowHeight_ = height;
  glViewport(0, 0, width, height);
}

}  // namespace agl
