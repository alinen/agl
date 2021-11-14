// Copyright 2009-2020, Savvy Sine, Aline Normoyle

#ifndef AGL_CAMERA_H_
#define AGL_CAMERA_H_

#include "agl/aglm.h"

namespace agl {

/**
 * @brief Implements orbit and pan camera movement
 * 
 * Default controls
 * * Click-drag the left mouse button to orbit
 * * Click-drag the middle mouse button to zoom
 * * Click-drag the right mouse button to pan
 * * Press spacebar to reset the view
 */
class Camera {
 public:
  Camera();
  virtual ~Camera();

  // Print eyepoint position and basis
  virtual void print();

  // Initialize the camera with gluLookAt parameters
  virtual void set(const glm::vec3& eyepos,
                   const glm::vec3& look = glm::vec3(0, 0, 0),
                   const glm::vec3& up = glm::vec3(0, 1, 0));

  /** @name Camera state
   */
  ///@{
  virtual const glm::mat4& viewMatrix() const;
  virtual const glm::vec3& position() const;
  virtual const glm::vec3& look() const;
  virtual const glm::vec3& up() const;
  virtual const glm::vec3& backward() const;
  virtual const glm::vec3& right() const;
  virtual float heading() const;
  virtual float pitch() const;
  ///@}

  /** @name Respond to user input 
   */
  ///@{
  void onMouseMotion(int x, int y);  // x and y in screen coords
  void onMouseButton(int button, int state, int x, int y, int mods);
  void onKeyboard(int key, int scancode, int action, int mods);
  void onScroll(float dx, float dy);
  ///@}

  /** @name Relative movement commands
   */
  ///@{
  void moveLeft(float scale = 1.0);
  void moveRight(float scale = 1.0);
  void moveUp(float scale = 1.0);
  void moveDown(float scale = 1.0);
  void moveForward(float scale = 1.0);
  void moveBack(float scale = 1.0);

  void turnLeft(float scale = 1.0);
  void turnRight(float scale = 1.0);
  void turnUp(float scale = 1.0);
  void turnDown(float scale = 1.0);

  void orbitLeft(float scale = 1.0);
  void orbitRight(float scale = 1.0);
  void orbitUp(float scale = 1.0);
  void orbitDown(float scale = 1.0);

  void setTurnRate(float rate);
  float turnRate() const;

  void setMoveSpeed(float rate);
  float moveSpeed() const;
  ///@}

 protected:
  virtual void turn(glm::vec3* v, glm::vec3* n, float amount);
  virtual void move(float dU, float dV, float dN);
  virtual void orbit(float h, float p);
  virtual void reset();
  virtual void _set(const glm::vec3& eyepos,
                    const glm::vec3& look,
                    const glm::vec3& up);

 protected:
  float mSpeed, mTurnRate;

  glm::vec3 mEye, mLook, mUp;  // camera position
  float mHeading, mPitch, mRadius;
  glm::vec3 mResetEye, mResetLook, mResetUp;  // cached camera position

  // Basis of camera local coord system
  glm::vec3 mU;  // up
  glm::vec3 mV;  // v points right
  glm::vec3 mN;  // -n points forward

  // Cache useful values
  glm::mat4 mCameraMatrix;

  // Mouse input
  mutable int mLastX, mLastY;
  int mButtonState;
  int mModifierState;
  int mActionState;
};

}  // namespace agl
#endif  // AGL_CAMERA_H_
