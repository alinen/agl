// Copyright 2020, Savvy Sine, alinen
// Convenience header for utilities and common includes 

#pragma once
#include <iostream>
#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <limits>
#include <memory>
#include <random>
#include <cmath>

extern std::ostream& operator<<(std::ostream& o, const glm::mat4& m);
extern std::ostream& operator<<(std::ostream& o, const glm::mat3& m);
extern std::ostream& operator<<(std::ostream& o, const glm::vec3& v);
extern std::ostream& operator<<(std::ostream& o, const glm::vec4& v);
extern std::ostream& operator<<(std::ostream& o, const glm::vec2& v);

namespace glm
{
   using point3 = glm::vec3;
   using color = glm::vec3;
}
const float kPI = glm::pi<float>();
const float kINFINITY = std::numeric_limits<float>::infinity();

namespace agl {

inline float random() 
{
   static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
   static std::mt19937 generator;
   return distribution(generator); 
}

inline float random(float min, float max) 
{
   static std::uniform_real_distribution<float> distribution(min, max);
   static std::mt19937 generator;
   return distribution(generator);
}

inline glm::vec3 randomUnitCube() 
{
   float x = random(-0.5,0.5);
   float y = random(-0.5,0.5);
   float z = random(-0.5,0.5);
   return glm::vec3(x, y, z);
}

inline glm::vec3 randomUnitSquare() 
{
   float x = random(-0.5,0.5);
   float y = random(-0.5,0.5);
   return glm::vec3(x, y, 0);
}


inline glm::vec3 randomUnitSphere() 
{
   glm::vec3 p = randomUnitCube();
   while (glm::length(p) >= 1.0f) 
   {
      p = randomUnitCube();
   } 
   return p;
}

inline glm::vec3 randomUnitDisk()
{
    glm::vec3 p = randomUnitSquare();
    while (glm::length(p) >= 1.0f)
    {
        p = randomUnitSquare();
    }
    return p;
}

// Generate random direction in hemisphere around normal
// from https://raytracing.github.io/books/RayTracingInOneWeekend.html (Peter Shirley)
inline glm::vec3 randomHemisphere(const glm::vec3& normal) 
{
   glm::vec3 in_unit_sphere = randomUnitSphere();
   if (glm::dot(in_unit_sphere, normal) > 0.0f) // In the same hemisphere as the normal
   {
       return in_unit_sphere;
   }
   else
   {
       return -in_unit_sphere;
   }
}

// Generate random unit vector
// from https://raytracing.github.io/books/RayTracingInOneWeekend.html (Peter Shirley)
inline glm::vec3 randomUnitVector() 
{
   return glm::normalize(randomUnitSphere());
}
} // end namespace ggl
