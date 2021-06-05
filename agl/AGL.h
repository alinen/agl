#if ( (defined(__MACH__)) && (defined(__APPLE__)) )
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif
