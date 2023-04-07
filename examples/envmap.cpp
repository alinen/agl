// Copyright (c) 2020, Savvy Sine, Aline Normoyle
#include "agl/window.h"

using glm::vec3;
class MyWindow : public agl::Window {
  void setup() {
    setWindowSize(1000, 1000);
    renderer.loadShader("envmap", 
      "../shaders/envmap.vs",
      "../shaders/envmap.fs");

    renderer.loadCubemap("background", "../textures/sea", 0);
    perspective(glm::radians<float>(60.0f), 1.0f, 0.1f, 100.0f);
  }

  void keyDown(int key, int mods) {
    if (key == 'F') {
      F = F + (mods == GLFW_KEY_LEFT_CONTROL? 0.01 : -0.01);
    }
    else if (key == 'E') {
      eta = eta + (mods == GLFW_KEY_LEFT_CONTROL? 0.01 : -0.01);
    }

  }

  void draw() {

    vec3 etaRatio(eta-0.1, eta-0.05, eta);
    renderer.beginShader("envmap");
    renderer.texture("cubemap", "background");
    renderer.setUniform("etaRatio", etaRatio);
    renderer.setUniform("fresnel", F);
    renderer.setUniform("fresnelPower", 2.0f);
    renderer.scale(vec3(1.5));
    renderer.sphere();
    renderer.endShader();

    renderer.beginShader("cubemap");
    renderer.texture("cubemap", "background");
    renderer.skybox(10);
    renderer.endShader();
  }
  float eta = 2.4f;
  float F = 0.1f;
};

int main() {
  MyWindow window;
  window.run();
}
