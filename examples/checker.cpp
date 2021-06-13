// Copyright 2020, Savvy Sine, Aline Normoyle

#include "agl/window.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;

class MyWindow : public agl::Window {
  void setup() {
    renderer.setUniform("Fog.enabled", true);
    renderer.setUniform("Fog.color", vec3(0.9f));
    renderer.setUniform("Fog.minDist", 5.0f);
    renderer.setUniform("Fog.maxDist", 9.0f);
    renderer.setUniform("Material.specular", vec3(0.0));
    renderer.setUniform("MainTexture.enabled", true);
    renderer.setUniform("MainTexture.tile", vec2(10.0));

    int halfw = 32;
    int halfh = 32;
    agl::Image checker(halfw*2, halfh*2);
    for (int i = 0; i < checker.height(); i++) {
      for (int j = 0; j < checker.width(); j++) {
        if ((i < halfh && j < halfw) || (i > halfh && j > halfw)) {
          checker.setVec4(i, j, vec4(0, 0, 0, 1));
        } else {
          checker.setVec4(i, j, vec4(1, 1, 1, 1));
        }
      }
    }
    renderer.loadTexture("checker", checker, 0);
    renderer.texture("MainTexture.texture", "checker");

    setupPerspectiveScene(vec3(0.0), vec3(10.0));
    background(vec3(0.9f));
  }

  void draw() {
    renderer.scale(vec3(20.0f));
    renderer.plane();
  }
};

int main() {
  MyWindow window;
  window.run();
}
