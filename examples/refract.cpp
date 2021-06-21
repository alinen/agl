// Copyright (c) 2020, Savvy Sine, Aline Normoyle
#include "agl/window.h"

using glm::vec3;
class MyWindow : public agl::Window {
  void setup() {
    renderer.loadShader("refract", 
      "../shaders/refract.vs",
      "../shaders/refract.fs");

    renderer.loadCubemap("background", "../textures/sea", 0);
    perspective(glm::radians<float>(60.0f), 1.0f, 0.1f, 100.0f);
  }

  void draw() {
    renderer.beginShader("refract");
    renderer.texture("background", "background");
    renderer.scale(vec3(1.5));
    renderer.teapot();
    renderer.endShader();

    renderer.beginShader("cubemap");
    renderer.texture("cubemap", "background");
    renderer.skybox(10);
    renderer.endShader();
  }
};

int main() {
  MyWindow window;
  window.run();
}
