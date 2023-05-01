// Copyright (c) 2020, Savvy Sine, Aline Normoyle
#include "agl/window.h"
#include <string>
#include <sstream>
#include <iomanip>

using glm::vec3;
class MyWindow : public agl::Window {
  void setup() {
    setWindowSize(1000, 1000);
    renderer.loadShader("envmap", 
      "../shaders/envmap.vs",
      "../shaders/envmap.fs");

    renderer.loadCubemap("background", "../textures/sea", 0);
    perspective(glm::radians<float>(60.0f), 1.0f, 0.1f, 100.0f);
    lookAt(vec3(0,0,2), vec3(0), vec3(0,1,0));
  }

  void checkKeyDown() {
    int mod = keyIsDown(GLFW_KEY_LEFT_SHIFT);
    float val = mod? 0.01 : -0.01;

    if (keyIsDown('F')) {
      FScale = FScale + val;
    }
    if (keyIsDown('B')) {
      FBias = FBias + val;
    }
    if (keyIsDown('E')) {
      etaRatio = etaRatio + vec3(val);
    }
    if (keyIsDown('1')) {
      etaRatio[0] = etaRatio[0] + val; 
    }
    if (keyIsDown('2')) {
      etaRatio[1] = etaRatio[1] + val;
    }
    if (keyIsDown('3')) {
      etaRatio[2] = etaRatio[2] + val;
    }
    if (keyIsDown('P')) {
      power += val;
    }

  }

  void draw() {

    checkKeyDown();

    renderer.beginShader("envmap");
    renderer.texture("cubemap", "background");
    renderer.setUniform("etaRatio", etaRatio); // eta/etaPrime
    renderer.setUniform("fresnelScale", FScale);
    renderer.setUniform("fresnelBias", FBias);
    renderer.setUniform("fresnelPower", power);
    renderer.scale(vec3(1.5));
    renderer.teapot();
    renderer.endShader();

    renderer.beginShader("cubemap");
    renderer.texture("cubemap", "background");
    renderer.skybox(10);
    renderer.endShader();

    renderer.fontSize(36);
    
    std::stringstream lines[4];
    lines[0] << std::fixed << std::setprecision(2) << "FScale (F): " << FScale; 
    lines[1] << std::fixed << std::setprecision(2) << "FBias (B): " << FBias; 
    lines[2] << std::fixed << std::setprecision(2) << "Power (P): " << power; 
    lines[3] << std::fixed << std::setprecision(2) << "etaRatio (E,1,2,3): " << 
      etaRatio[0] <<  " " << etaRatio[1] << " "<< etaRatio[2];

    for (int i = 0; i < 4; i++) {
      renderer.text(lines[i].str(), 25, 27 * (i + 1));
    }
  }
  float eta = 2.07f;
  vec3 etaRatio = vec3(eta-0.2, eta-0.1, eta);
  float FScale = 1.22f;
  float FBias = 0.55f;
  float power = 4.0f;
};

int main() {
  MyWindow window;
  window.run();
}
