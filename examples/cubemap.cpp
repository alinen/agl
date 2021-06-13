// Copyright (c) 2020, savvy sine, Aline Normoyle, alinen
// Visualize a cubemap texture using the skybox primitive

#include "agl/window.h"

class MyWindow : public agl::Window
{
  void setup() {
    renderer.loadCubemap("cubemap", "../textures/sea", 0);
    perspective(glm::radians<float>(60.0f), 1.0f, 0.1f, 100.0f);
  }  
  void draw()
  {
    renderer.beginShader("cubemap");
    renderer.skybox(10);
    renderer.endShader();
  }
};

int main()
{
  MyWindow window;
  window.run();
}
