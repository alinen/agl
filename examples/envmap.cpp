// Copyright (c) 2020, Savvy Sine, Aline Normoyle
#include "agl/window.h"
#include <format>
#include <string>

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
    float val = (mods == GLFW_KEY_LEFT_CONTROL)? 0.01 : -0.01;
    if (key == '1') {
      FScale = FScale + val;
    }
    if (key == '2') {
      FBias = FBias + val;
    }
    else if (key == 'E') {
      etaRatio = etaRatio + vec3(val);
    }
    else if (key == 'R') {
      etaRatio[0] = etaRatio[0] + val; 
    }
    else if (key == 'G') {
      etaRatio[1] = etaRatio[1] + val;
    }
    else if (key == 'B') {
      etaRatio[2] = etaRatio[2] + val;
    }

  }

  void draw() {

    renderer.beginShader("envmap");
    renderer.texture("cubemap", "background");
    renderer.setUniform("etaRatio", etaRatio); // eta/etaPrime
    renderer.setUniform("fresnelScale", FScale);
    renderer.setUniform("fresnelBias", FBias);
    renderer.setUniform("fresnelPower", 2.0f);
    renderer.scale(vec3(1.5));
    renderer.rotate(glm::radians(90.0f), vec3(1, 0, 0));
    renderer.plane();
    renderer.endShader();

    renderer.beginShader("cubemap");
    renderer.texture("cubemap", "background");
    renderer.skybox(10);
    renderer.endShader();

    renderer.fontSize(36);
    /*
    std::string lines[3];
    lines[0] = "FScale: " + FScale; 
    lines[1] = "FBias: " + FBias; 
    lines[2] = "etaRatio: {:.2f}, {:.2f}, {:.2f}", etaRatio[0], etaRatio[1], etaRatio[2]);

    for (int i = 0; i < 3; i++) {
      renderer.text(lines[i], 25, 25 * (i + 1));
    }*/
  }
  float eta = 2.4f;
  vec3 etaRatio = vec3(eta-0.6, eta-0.4, eta);
  float FScale = 0.9f;
  float FBias = 0.1f;
};

int main() {
  MyWindow window;
  window.run();
}
