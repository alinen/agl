// Copyright 2009-2020, Savvy Sine, alinen, Aline Normoyle

#include "agl/camera.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "agl/aglm.h"
#include "agl/agl.h"

namespace agl {
using glm::vec3;
using glm::mat4;

Camera::Camera() :
  mSpeed(0.05),
  mTurnRate(0.01),
  mEye(0.0, 2.0, 2.5),
  mLook(0.0, 0.0, 0.0),
  mUp(0.0, 1.0, 0.0),
  mHeading(0.0),
  mPitch(0.0),
  mRadius(0.0) {
  set(mEye, mLook, mUp);
}

Camera::~Camera() {
}

const vec3& Camera::up() const {
  return mU;
}

const vec3& Camera::backward() const {
  return mN;
}

const vec3& Camera::right() const {
  return mV;
}

const mat4& Camera::viewMatrix() const {
  return mCameraMatrix;
}

const vec3& Camera::look() const {
  return mLook;
}

const vec3& Camera::position() const {
  return mEye;
}

float Camera::heading() const {
  return mHeading;
}

float Camera::pitch() const {
  return mPitch;
}

void Camera::reset() {
  _set(mResetEye, mResetLook, mResetUp);
}

void Camera::set(const vec3& eyepos, const vec3& look, const vec3& up) {
  mResetEye = eyepos;
  mResetLook = look;
  mResetUp = up;

  _set(eyepos, look, up);

  mPitch = atan2(-mN[1], -mN[2]);
  mHeading = atan2(-mN[2], -mN[0]);
}

void Camera::_set(const vec3& eyepos, const vec3& look, const vec3& up) {
  mEye = eyepos;
  mLook = look;
  mN = normalize(eyepos - look);
  mV = cross(up, mN);
  mU = cross(mN, mV);
  mRadius = distance(eyepos, look);  // cache this distance

  mU = normalize(mU);
  mV = normalize(mV);
  mN = normalize(mN);

  mCameraMatrix = glm::lookAt(eyepos, look, up);
}

void Camera::print() {
  std::cout << "EYE: " << mEye << std::endl;
  std::cout << "RIGHT: " << mV << std::endl;
  std::cout << "UP: " << mU << std::endl;
  std::cout << "N: " << mN << std::endl;
  std::cout << "Pitch: " << mPitch << std::endl;
  std::cout << "Heading: " << mHeading << std::endl;
  std::cout << "-----------------------\n";
}

void Camera::move(float dV, float dU, float dN) {
  mEye += dU*mU + dV*mV + dN*mN;
  mLook += dU*mU + dV*mV + dN*mN;
  _set(mEye, mLook, mUp);
}

void Camera::orbit(float h, float p) {
  // printf("PITCH: %f\n", p);
  // printf("HEADING: %f\n", h);
  // printf("RADIUS: %f\n", mRadius);

  vec3 rotatePt;  // Calculate new location around sphere having mRadius
  if (mUp == vec3(0, 1, 0)) {
     // Y UP
     rotatePt[0] = mRadius*cos(h)*cos(p);
     rotatePt[1] = mRadius*sin(p);
     rotatePt[2] = mRadius*sin(h)*cos(p);
  } else {
     rotatePt[0] = mRadius*cos(h)*cos(p);
     rotatePt[1] = mRadius*sin(h)*cos(p);
     rotatePt[2] = mRadius*sin(p);
  }

  _set(mLook-rotatePt, mLook /* look */, mUp /* up Approx */);
}

void Camera::orbitLeft(float scale) {
  mHeading += mTurnRate * scale;
  orbit(mHeading, pitch());
}

void Camera::moveLeft(float scale) {  // => move along v
  move(-mSpeed * scale, 0.0, 0.0);
}

void Camera::orbitRight(float scale) {
  mHeading -= mTurnRate * scale;
  orbit(mHeading, pitch());
}

void Camera::moveRight(float scale) {  // => move along v
  move(mSpeed * scale, 0.0, 0.0);
}

void Camera::orbitUp(float scale) {
  mPitch = mPitch + mTurnRate * scale;
  // mPitch = std::min<float>(-0.1, mPitch + mTurnRate * scale);
  orbit(heading(), mPitch);
}

void Camera::moveUp(float scale) {  // => move along +u
  move(0.0, mSpeed * scale, 0.0);
}

void Camera::orbitDown(float scale) {
  mPitch = mPitch - mTurnRate * scale;
  // mPitch = std::max<float>(-M_PI / 2.0 + 0.01, mPitch - mTurnRate * scale);
  orbit(heading(), mPitch);
}

void Camera::moveDown(float scale) {  // => move along -u
  move(0.0, -mSpeed * scale, 0.0);
}

void Camera::moveForward(float scale) {  // => move along -n
  mRadius += -mSpeed * scale;  // Also "zoom" into radius
  orbit(heading(), pitch());
}

void Camera::moveBack(float scale) {  // => move along n
  mRadius += mSpeed * scale;  // Also "zoom" out radius
  orbit(heading(), pitch());
}

void Camera::turn(vec3* v1, vec3* v2, float amount) {
  double cosTheta = cos(amount);
  double sinTheta = sin(amount);

  float vX =  cosTheta * v1->x + sinTheta * v2->x;
  float vY =  cosTheta * v1->y + sinTheta * v2->y;
  float vZ =  cosTheta * v1->z + sinTheta * v2->z;

  float nX = -sinTheta * v1->x + cosTheta * v2->x;
  float nY = -sinTheta * v1->y + cosTheta * v2->y;
  float nZ = -sinTheta * v1->z + cosTheta * v2->z;

  *v1 = vec3(vX, vY, vZ);
  *v2 = vec3(nX, nY, nZ);
}

void Camera::turnLeft(float scale) {  // rotate around u
  turn(&mV, &mN, -mTurnRate * scale);
}

void Camera::turnRight(float scale) {  // rotate neg around u
  turn(&mV, &mN, mTurnRate * scale);
}

void Camera::turnUp(float scale) {  // rotate around v
  turn(&mN, &mU, mTurnRate * scale);
}

void Camera::turnDown(float scale) {  // rotate around v
  turn(&mN, &mU, -mTurnRate * scale);
}

void Camera::setTurnRate(float rate) {
  mTurnRate = rate;
}

float Camera::turnRate() const {
  return mTurnRate;
}

void Camera::setMoveSpeed(float rate) {
  mSpeed = rate;
}

float Camera::moveSpeed() const {
  return mSpeed;
}

void Camera::onMouseMotion(int pX, int pY) {
  // https://www.glfw.org/docs/latest/input_guide.html
  if (mActionState != GLFW_PRESS) return;

  int deltaX = mLastX - pX;
  int deltaY = mLastY - pY;
  bool moveLeftRight = abs(deltaX) > abs(deltaY);
  bool moveUpDown = !moveLeftRight;

  if (mButtonState == GLFW_MOUSE_BUTTON_LEFT && mModifierState == 0x0) {  // Rotate
    if (moveLeftRight && deltaX > 0) orbitLeft(deltaX);
    else if (moveLeftRight && deltaX < 0) orbitRight(-deltaX);
    else if (moveUpDown && deltaY > 0) orbitUp(deltaY);
    else if (moveUpDown && deltaY < 0) orbitDown(-deltaY);

  } else if (mButtonState == GLFW_MOUSE_BUTTON_MIDDLE) {  // Zoom
    if (moveUpDown && deltaY > 0) moveForward(deltaY);
    else if (moveUpDown && deltaY < 0) moveBack(-deltaY);

  } else if (mButtonState == GLFW_MOUSE_BUTTON_RIGHT) {  // Pan
    if (moveLeftRight && deltaX > 0) moveLeft(deltaX);
    else if (moveLeftRight && deltaX < 0) moveRight(-deltaX);
    else if (moveUpDown && deltaY > 0) moveUp(deltaY);
    else if (moveUpDown && deltaY < 0) moveDown(-deltaY);
  }

  mLastX = pX;
  mLastY = pY;
}

void Camera::onMouseButton(int pButton, int pState, int x, int y, int mods) {
  mButtonState = pButton;
  mActionState = pState;
  mModifierState = mods;
  mLastX = x;
  mLastY = y;
}

void Camera::onScroll(float dx, float dy) {
  if (dy > 0) {
    moveForward(dy * 10.0);
  } else {
    moveBack(-dy * 10.0);
  }
}

void Camera::onKeyboard(int pKey, int scancode, int action, int mods) {
  if (pKey == ' ') {
    reset();
  } 
}
}  // namespace agl
