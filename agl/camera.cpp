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
  mTurnRate(1.0*(M_PI / 180.0)),
  mEye(0.0, 2.0, 2.5),
  mLook(0.0, 0.0, 0.0),
  mUp(0.0, 1.0, 0.0),
  mHeading(-0.85),
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
}

void Camera::_set(const vec3& eyepos, const vec3& look, const vec3& up) {
  mEye = eyepos;
  mN = eyepos - look;
  mV = cross(up, mN);
  mU = cross(mN, mV);
  mRadius = length(mN); // cache this distance

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
  std::cout << "RIGHT-----------------------\n";
}

void Camera::move(float dV, float dU, float dN) {
  mEye += dU*mU + dV*mV + dN*mN;
  mLook += dU*mU + dV*mV + dN*mN;
  _set(mEye, mLook, mUp);
}

void Camera::orbit(float h, float p) {
  //printf("PITCH: %f\n", p);
  //printf("HEADING: %f\n", h);
  //printf("RADIUS: %f\n", mRadius);

  vec3 rotatePt; // Calculate new location around sphere having mRadius
  if (mUp == vec3(0,1,0)) {
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
  //mPitch = std::min<float>(-0.1, mPitch + mTurnRate * scale);
  orbit(heading(), mPitch);
}

void Camera::moveUp(float scale) {  // => move along +u
  move(0.0, mSpeed * scale, 0.0);
}

void Camera::orbitDown(float scale) {
  mPitch = mPitch - mTurnRate * scale;
  //mPitch = std::max<float>(-M_PI / 2.0 + 0.01, mPitch - mTurnRate * scale);
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

void Camera::turn(vec3& v1, vec3& v2, float amount) {
  double cosTheta = cos(amount);
  double sinTheta = sin(amount);

  float vX =  cosTheta*v1[0] + sinTheta*v2[0];
  float vY =  cosTheta*v1[1] + sinTheta*v2[1];
  float vZ =  cosTheta*v1[2] + sinTheta*v2[2];

  float nX = -sinTheta*v1[0] + cosTheta*v2[0];
  float nY = -sinTheta*v1[1] + cosTheta*v2[1];
  float nZ = -sinTheta*v1[2] + cosTheta*v2[2];

  v1 = vec3(vX, vY, vZ);
  v2 = vec3(nX, nY, nZ);
}

void Camera::turnLeft(float scale) {  // rotate around u
  turn(mV, mN, -mTurnRate * scale);
}

void Camera::turnRight(float scale) {  // rotate neg around u
  turn(mV, mN, mTurnRate * scale);
}

void Camera::turnUp(float scale) {  // rotate around v
  turn(mN, mU, mTurnRate * scale);
}

void Camera::turnDown(float scale) {  // rotate around v
  turn(mN, mU, -mTurnRate * scale);
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
  if (mModifierState != GLFW_PRESS) return;

  int deltaX = mLastX - pX;
  int deltaY = mLastY - pY;
  bool moveLeftRight = abs(deltaX) > abs(deltaY);
  bool moveUpDown = !moveLeftRight;

  if (mButtonState == GLFW_MOUSE_BUTTON_LEFT) {  // Rotate
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

void Camera::onMouseButton(int pButton, int pState, int x, int y) {
  mButtonState = pButton;
  mModifierState = pState;
  mLastX = x;
  mLastY = y;
}

void Camera::onScroll(float dx, float dy) {
  if (dy > 0) moveForward(dy * 10.0);
  else moveBack(-dy * 10.0);
}

void Camera::onKeyboard(int pKey, int scancode, int action, int mods) {
  if (pKey == ' ') {
    reset();
  }
  // AN TODO frameVolume(vec3(0.0), vec3(100,200,100));
}

void Camera::frameVolume(const vec3& pos, const vec3& dim)
{
  double w = dim[0];
  double h = dim[1];
  double d = dim[2];
  double vfov = 60.0;
  double angle = 0.5 * vfov * M_PI / 180.0;
  double dist;
  if (d > h) {
    mUp = vec3(0, 0.0, 1.0);
    if (w > d) dist = w*0.5 / tan(angle);  // aspect is 1, so i can do this
    else dist = d*0.5 / tan(angle);
    mEye = mEye + pos;
    mLook = pos + vec3(0, 0.0, d*0.75);
  } else {
    mUp = vec3(0, 1.0, 0.0);
    if (w > h) dist = w*0.5 / tan(angle);  // aspect is 1, so i can do this
    else dist = h*0.5 / tan(angle);
    mEye = vec3(-(dist + d), h*0.1, (dist + d));
    mEye = mEye + pos;
    mLook = pos + vec3(0, h*0.5, 0);
  }
  set(mEye, mLook, mUp);
}

}  // namespace agl
