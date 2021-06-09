// Copyright 2020, Savvy Sine, alinen

#ifndef AGL_WINDOW_H_
#define AGL_WINDOW_H_


#include <string>
#include <map>
#include "agl/agl.h"
#include "agl/aglm.h"
#include "agl/renderer.h"
#include "agl/camera.h"

namespace agl {

/**
 * @brief The window class manages the window and user input.
 * 
 * Override this class to create a custom application.
 * @verbinclude sphere.cpp
 */ 
class Window {
 public:
  /**
   * @brief Constructor.
   * Override this class to create a custom application.
   * The default window is sized 500x500 and draws an empty (black) scene.
   * @verbinclude empty.cpp
   */
  Window();
  virtual ~Window();

  /**
   * @brief Opens the window and starts the main application loop.
   * @verbinclude run.cpp
   *
   * This function should typically called from main after creating the Window
   * object. This function invokes the user's setup() function and then 
   * repeatedly calls the user's draw() function. This function returns when 
   * the user closes the window (via the escape key or close menu button)
   */
  void run();

  /**
   * @brief Save the current screen image to a file 
   * @verbinclude screenshot.cpp
   * @param filename image file name (should be a .png file)
   * @return (bool) Returns false if the image cannot be saved; true otherwise
   *
   * Filenames should include the png file extension. 
   * Image with relative paths will be written relative to the directory
   * from which you run the executable. Images are saved in RGBA format.
   */
  bool screenshot(const std::string& filename);

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
  void setSize(int w, int h);
  void background(const glm::vec3& color);

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
  Camera _camera;
  struct GLFWwindow* window_ = 0;
};

}  // namespace agl
#endif  // AGL_WINDOW_H_
