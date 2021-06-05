// Copyright, 2020, Savvy Sine, alinen
//

#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include "agl/AGL.h"
#include "agl/AGLM.h"
#include "agl/renderer.h"
#include "agl/mesh.h"

using namespace std;
using namespace glm;
using namespace agl;

int LeftButtonDown = 0;
int ShiftLeftButtonDown = 0;           
int OldX,OldY,NewX,NewY;
float Elevation=0;
float Azimuth=0;
float Dist = 10;  


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (action != GLFW_PRESS) return;

   if (key == GLFW_KEY_ESCAPE)
   {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
   }
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   // Prevent a divide by zero
   if (height == 0) height = 1;
   glViewport(0, 0, width, height);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
   double xpos, ypos;
   glfwGetCursorPos(window, &xpos, &ypos);

   NewX = xpos; 
   NewY = ypos;

   int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
   if (state == GLFW_PRESS)
   {
       LeftButtonDown = 1;
       int keyPress = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
       if (keyPress == GLFW_PRESS) ShiftLeftButtonDown = 1;
   }
   else if (state == GLFW_RELEASE)
   {
       ShiftLeftButtonDown = LeftButtonDown = 0;
   }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
   if (!LeftButtonDown) return;

    float RelX, RelY;

    OldX = NewX; 
    OldY = NewY;
    NewX = xpos;    
    NewY = ypos;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    RelX = (NewX - OldX) / (float)width;
    RelY = (NewY - OldY) / (float)height;

    if (ShiftLeftButtonDown) 
    {
        // Change distance
        Dist += RelY*4;
    } 
    else if (LeftButtonDown) 
    { 
	     // Change Rotation
        Azimuth += (RelX);
        Elevation += (RelY);
    }
}

int main(int argc, char** argv)
{
   GLFWwindow* window;

   if (!glfwInit())
   {
      return -1;
   }

   // Explicitly ask for a 4.0 context 
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   /* Create a windowed mode window and its OpenGL context */
   float width = 500;
   float height = 500;
   window = glfwCreateWindow(width, height, "Particle Viewer", NULL, NULL);
   if (!window)
   {
      glfwTerminate();
      return -1;
   }

   glfwMakeContextCurrent(window);
   glfwSetKeyCallback(window, key_callback);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
   glfwSetMouseButtonCallback(window, mouse_button_callback);
   glfwSetScrollCallback(window, scroll_callback);
   glfwSetCursorPosCallback(window, cursor_position_callback);

#ifndef APPLE
   if (glewInit() != GLEW_OK)
   {
      cout << "Cannot initialize GLEW\n";
      return -1;
   }
#endif

   glClearColor(0, 0, 0, 1);

   Mesh mesh;
   mesh.loadPLY("../models/airplane.ply");
   float fov = radians(30.0f);

   Renderer renderer;
   renderer.init();
   renderer.perspective(fov, 1.0f, 0.1f, 100.0f);

   float lastTime = glfwGetTime();
   while (!glfwWindowShouldClose(window))
   {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers

      float dt = glfwGetTime() - lastTime;
      lastTime = glfwGetTime();

      float z = (Dist) * cos(Azimuth) * cos(Elevation);
      float y = (Dist) * sin(Elevation);
      float x = (Dist) * sin(Azimuth) * cos(Elevation);
      renderer.lookAt(vec3(x, y, z), vec3(0, 0, 0));

      renderer.skybox();
      renderer.mesh(glm::scale(mat4(1.0), vec3(0.1)), mesh);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glfwTerminate();
   return 0;
}

