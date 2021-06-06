// Copyright, 2019, Savvy Sine, alinen

#ifndef AGL_AGL_H_
#define AGL_AGL_H_

#if ( (defined(__MACH__)) && (defined(__APPLE__)) )
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#endif  // AGL_AGL_H_
