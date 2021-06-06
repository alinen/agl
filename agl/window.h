// Copyright 2020, Savvy Sine, alinen

#ifndef AGL_WINDOW_H_
#define AGL_WINDOW_H_

#include <map>
#include "agl/agl.h"
#include "agl/aglm.h"
#include "agl/renderer.h"

namespace agl {

class Window {
 public:
  explicit Window(int width = 600, int height = 600);
  virtual ~Window();
  void run();

 protected:
  // Overridable user interface
  virtual void setup() {}
  virtual void update() {}
  virtual void draw() {}
  virtual void initializeGui() {}
  virtual void mouseMotion(int x, int y) {}
  virtual void mousePress(int button, int mods) {}
  virtual void scroll(float dx, float dy) {}
  virtual void mouseRelease(int button, int mods) {}
  virtual void keyUp(int key, int mods) {}
  virtual void keyDown(int key, int mods) {}

  // Query input and time
  // https://www.glfw.org/docs/latest/group__keys.html
  // pass either a keycode or ascii uppercase letter or digit
  bool keyIsDown(int key) const;
  bool mouseIsDown(int button) const;
  int mouseX() const;
  int mouseY() const;
  float dt() const;  // amount of time since last frame
  float elapsedTime() const;  // amount of time since start (can be reset)
  float height() const;
  float width() const;

  // rendering
  void setBackgroundColor(const glm::vec3& c);

 private:
  void init();

  static void onScrollCb(GLFWwindow* w, double xoffset, double yoffset);
  static void onMouseMotionCb(GLFWwindow* w, double x, double y);
  static void onMouseButtonCb(GLFWwindow* w, int button, int action, int mods);
  static void onResizeCb(GLFWwindow* w, int width, int height);
  static void onKeyboardCb(GLFWwindow* w,
    int key, int code, int action, int mods);

  void onMouseMotion(int x, int y);
  void onMouseButton(int button, int action, int mods);
  void onKeyboard(int key, int scancode, int action, int mods);
  void onResize(int width, int height);
  void onScroll(float xoffset, float yoffset);

 protected:
  Renderer renderer;

 private:
  int windowWidth_, windowHeight_;
  float elapsedTime_;
  float dt_;
  struct GLFWwindow* window_ = 0;
};

}  // namespace agl

#endif  // AGL_WINDOW_H_
