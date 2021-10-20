// Copyright 2020, Savvy Sine, Aline Normoyle

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
 * @brief Manages the window and user input.
 *
 * Override this class to create a custom application.
 * @verbinclude sphere.cpp
 */
class Window {
 public:
  /**
   * @brief Constructor.
   *
   * Override this class to create a custom application.
   * The default window is sized 500x500 and draws an empty (black) scene.
   * @verbinclude empty.cpp
   */
  Window();
  virtual ~Window();

  /**
   * @brief Opens the window and starts the main application loop.
   *
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
   *
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
  /** @name Respond to events
   */
  ///@{
  /**
   * @brief Override this method to perform setup before the main application
   * loop
   *
   * Use this method to load textures, load meshes, set the window size, and
   * initialize the projection and view.
   *
   * @verbinclude plane.cpp
   *
   * @see setWindowSize(int,int)
   * @see setupPerspectiveScene(const glm::vec3&, const glm::vec3&)
   * @see setupOrthoScene(const glm::vec3&, const glm::vec3&)
   * @see Renderer.loadTexture(const std::string&, const std::string&, int)
   * @see Renderer.loadCubemap(const std::string&, const std::string&, int)
   */
  virtual void setup() {}

  /**
   * @brief Override this method to draw
   *
   * @verbinclude plane.cpp
   */
  virtual void draw() {}

  /**
   * @brief Override this method to respond to mouse movement
   * @param x The current x position
   * @param y The current y position
   * @param dx The change in x since the last mouse event (in pixels)
   * @param dy The change in y since the last mouse event (in pixels)
   *
   * Screen coordinates are in pixels. x values are always between 0 and width.
   * y values are always between 0 and height.
   */
  virtual void mouseMotion(int x, int y, int dx, int dy) {}

  /**
   * @brief Override this method to respond to mouse press (button down)
   * @param button The mouse button that is pressed, e.g. GLFW_MOUSE_BUTTON_LEFT
   * @param mods Modifiers that are pressed (e.g. shift, control, etc)
   *
   * @ref https://www.glfw.org/docs/latest/input_guide.html
   * @see mouseUp(int, int)
   * @see mouseIsDown(int)
   */
  virtual void mouseDown(int button, int mods) {}

  /**
   * @brief Override this method to respond to mouse press (button up)
   *
   * @see mouseDown(int, int)
   * @see mouseIsDown(int)
   */
  virtual void mouseUp(int button, int mods) {}

  /**
   * @brief Override this method to respond to scrolling the middle mouse
   * button
   * @param dx The change in the x direction (in scroll coordinates)
   * @param dy The change in the x direction (in scroll coordinates)
   *
   * @verbinclude plane.cpp
   */
  virtual void scroll(float dx, float dy) {}

  /**
   * @brief Override this method to respond to key presses (button up)
   * @param key The key, e.g. as either a constant or capital ascii letter,
   * such as 'P'. 
   * @param mods Modifiers that are pressed (e.g. shift, control, etc)
   *
   * @ref https://www.glfw.org/docs/latest/group__keys.html
   * @verbinclude camera_controls.cpp
   * @see keyDown(int,int)
   * @see keyIsDown(int)
   */
  virtual void keyUp(int key, int mods) {}

  /**
   * @brief Override this method to respond to key presses (button down)
   * @param key The key, e.g. as either a constant or capital ascii letter,
   * such as 'P'. 
   * @param mods Modifiers that are pressed (e.g. shift, control, etc)
   *
   * @ref https://www.glfw.org/docs/latest/group__keys.html
   * @see keyUp(int,int)
   */
  virtual void keyDown(int key, int mods) {}

  /**
   * @brief Override this method to respond to window resizing
   * @param width The new window width 
   * @param height The new window height 
   * 
   * \verbinclude shadertoy.cpp
   */
  virtual void resize(int width, int height) {}
  ///@}

  /** @name Query input and time
   */
  ///@{
  /** 
   * @brief Query whether the given key is down 
   * @key The key to test
   * 
   * More than one key can be pressed at once. The key can be an ascii 
   * capital letter or a GLFW constant
   * @ref https://www.glfw.org/docs/latest/input_guide.html
   */
  bool keyIsDown(int key) const;

  /** 
   * @brief Query whether the given mouse button is down
   * @param button The button to test
   * 
   * More than one button can be pressed at once. The button should be a GLFW
   * constant, such as GLFW_MOUSE_BUTTON_LEFT
   * @ref https://www.glfw.org/docs/latest/input.html
   */
  bool mouseIsDown(int button) const;

  /** 
   * @brief Return the current mouse position (in screen coordinates)
   */
  glm::vec2 mousePosition() const;

  /** 
   * @brief Return the amount of time since the previous frame (in seconds)
   *
   * If the frame rate is 30 frames per second, dt would be approximately 1/30
   * = 0.033333 seconds each frame
   */
  float dt() const;  // amount of time since last frame

  /** 
   * @brief Return the amount of time since the setup() was called (in seconds)
   */
  float elapsedTime() const;  // amount of time since start (can be reset)

  /** 
   * @brief Return the window height in pixels
   */
  float height() const;

  /** 
   * @brief Return the window width in pixels
   */
  float width() const;
  ///@}

  /** 
   * @brief Stop the main application loop
   * 
   * Call this function to terminate the main loop, either from setup() or 
   * draw()
   */
  void noLoop();

  /** 
   * @brief Set the background color
   * 
   * This function can be called from draw and will additionally clear the
   * color and depth buffers
   */
  void background(const glm::vec3& color);

  /** @name Projections and view
   */
  ///@{
  /**
   * @brief Set the size of the window in pixels.
   *
   * @see width
   * @see height
   */
  void setWindowSize(int w, int h);

  /**
   * @brief Initialize the projection and camera to fit the given dimensions
   * and center using an orthographic projection
   * @param center The center of the scene
   * @param dim The dimension of the scene
   *
   * @see ortho(float, float, float, float, float, float)
   * @see lookAt(const glm::vec3, const glm::vec3&, const glm::vec3&)
   */
  void setupOrthoScene(const glm::vec3& center, const glm::vec3& dim);

  /**
   * @brief Initialize the projection and camera to fit the given dimensions
   * and center using a perspective projection
   * @param center The center of the scene
   * @param dim The dimension of the scene
   *
   * @see perspective(float, float, float, float)
   * @see lookAt(const glm::vec3, const glm::vec3&, const glm::vec3&)
   */
  void setupPerspectiveScene(const glm::vec3& center, const glm::vec3& dim);

  /**
   * @copydoc Renderer::lookAt(
   *     const glm::vec3&, const glm::vec&, const glm::vec3&)
   *
   * If you're not using the default camera controls and you don't call
   * setupSecene, the camera is at position (0,0,2) and faces the point 
   * (0,0,0)
   *
   * @see setupPerspectiveScene(const glm::vec3&, const glm::vec3&)
   * @see setupOrthoScene(const glm::vec3&, const glm::vec3&)
   */
  void lookAt(const glm::vec3& camPos,
              const glm::vec3& camLook,
              const glm::vec3& up = glm::vec3(0, 1, 0));

  /**
   * @copydoc Renderer::perspective(float, float, float, float)
   *
   * @see setupPerspectiveScene(const glm::vec3&, const glm::vec3&)
   */
  void perspective(float fovRadians, float aspect, float near, float far);

  /**
   * @copydoc Renderer::ortho(float, float, float, float, float, float)
   *
   * @see setupOrthoScene(const glm::vec3&, const glm::vec3&)
   */
  void ortho(float minx, float maxx,
      float miny, float maxy, float minz, float maxz);

  /**
   * @brief Returns true if the camera controls are active; false otherwise
   * @see Camera
   */
  inline bool cameraEnabled() const { return _cameraEnabled; }

  /**
   * @brief Set whether the window's camera controls are active
   * @see Camera
   */
  inline void setCameraEnabled(bool on) { _cameraEnabled = on; }
  ///@}


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
  Camera camera;

 private:
  int _windowWidth, _windowHeight;
  float _elapsedTime;
  float _dt;
  float _lastx, _lasty;
  bool _cameraEnabled;
  glm::vec3 _backgroundColor;
  struct GLFWwindow* _window = 0;

 protected:
  inline GLFWwindow* window() const { return _window; }
};

}  // namespace agl
#endif  // AGL_WINDOW_H_
