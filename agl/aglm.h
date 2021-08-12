// Copyright 2020, Savvy Sine, alinen

#ifndef AGL_AGLM_H_
#define AGL_AGLM_H_

#include <stdio.h>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/epsilon.hpp>

/** 
 * @file aglm.h
 *
 * @brief Global utilities and common third party includes
 */


/** 
 * @brief Print a matrix
 *
 * ```
 * std::cout << mat4(1.0) << std::endl;
 * ```
 */ 
extern std::ostream& operator<<(std::ostream& o, const glm::mat4& m);

/** 
 * @brief Print a matrix
 *
 * ```
 * std::cout << mat3(1.0) << std::endl;
 * ```
 */ 
extern std::ostream& operator<<(std::ostream& o, const glm::mat3& m);

/** 
 * @brief Print a vector
 *
 * ```
 * std::cout << vec3(1.0, -3.0, 50.0) << std::endl;
 * ```
 */ 
extern std::ostream& operator<<(std::ostream& o, const glm::vec3& v);

/** 
 * @brief Print a vector
 *
 * ```
 * std::cout << vec4(0.2, 0.3, 0.8, 1.0) << std::endl;
 * ```
 */ 
extern std::ostream& operator<<(std::ostream& o, const glm::vec4& v);

/** 
 * @brief Print a vector
 *
 * ```
 * std::cout << vec2(-2.0, 1.0) << std::endl;
 * ```
 */ 
extern std::ostream& operator<<(std::ostream& o, const glm::vec2& v);

/** 
 * @brief Print a quaternion
 *
 * ```
 * quat q(m);
 * std::cout << q << std::endl;
 * ```
 */ 
extern std::ostream& operator<<(std::ostream& o, const glm::quat& v);

namespace glm {
  using point3 = glm::vec3;
  using color = glm::vec3;
}

const float kPI = glm::pi<float>();
const float kINFINITY = std::numeric_limits<float>::infinity();

namespace agl {

/** 
 * @brief Return a random number between 0 and 1 [0, 1)
 */ 
inline float random() {
  static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
  static std::mt19937 generator;
  return distribution(generator);
}

/** 
 * @brief Return a random number between min and max [min, max)
 */ 
inline float random(float min, float max) {
  static std::uniform_real_distribution<float> distribution(min, max);
  static std::mt19937 generator;
  return distribution(generator);
}

/** 
 * @brief Return a random direction in a unit cube
 */ 
inline glm::vec3 randomUnitCube() {
  float x = random(-0.5, 0.5);
  float y = random(-0.5, 0.5);
  float z = random(-0.5, 0.5);
  return glm::vec3(x, y, z);
}

/** 
 * @brief Return a random direction in a unit square in the XY plane
 */ 
inline glm::vec3 randomUnitSquare() {
  float x = random(-0.5, 0.5);
  float y = random(-0.5, 0.5);
  return glm::vec3(x, y, 0);
}


/** 
 * @brief Return a random direction in a unit sphere
 */ 
inline glm::vec3 randomUnitSphere() {
  glm::vec3 p = randomUnitCube();
  while (glm::length(p) >= 1.0f) {
    p = randomUnitCube();
  }
  return p;
}

/** 
 * @brief Return a random direction in a unit disk in the XY plane
 */ 
inline glm::vec3 randomUnitDisk() {
  glm::vec3 p = randomUnitSquare();
  while (glm::length(p) >= 1.0f) {
    p = randomUnitSquare();
  }
  return p;
}

/** 
 * @brief Generate random direction in hemisphere around normal 
 * @author Peter Shirley
 * @link https://raytracing.github.io/books/RayTracingInOneWeekend.html
 */ 
inline glm::vec3 randomHemisphere(const glm::vec3& normal) {
  glm::vec3 in_unit_sphere = randomUnitSphere();

  // In the same hemisphere as the normal
  if (glm::dot(in_unit_sphere, normal) > 0.0f) {
    return in_unit_sphere;
  } else {
    return -in_unit_sphere;
  }
}

/** 
 * @brief Generate random unit vector
 * @author Peter Shirley
 * @link https://raytracing.github.io/books/RayTracingInOneWeekend.html
 */ 
inline glm::vec3 randomUnitVector() {
  return glm::normalize(randomUnitSphere());
}

}  // end namespace agl
#endif  // AGL_AGLM_H_
