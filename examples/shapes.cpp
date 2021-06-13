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

    setupPerspectiveScene(vec3(0.0), vec3(10.0));
    background(vec3(0.9f));
  }

  void draw() {
    renderer.setUniform("MainTexture.enabled", false);
    renderer.texture("MainTexture.texture", "checker");
    renderer.identity();
    renderer.translate(vec3(0));
    renderer.plane();

/*
    renderer.identity();
    renderer.translate(vec3(-1,0.5,1));
    renderer.cone();

    renderer.identity();
    renderer.translate(vec3(0,0.5,1));
    renderer.sphere();

    renderer.identity();
    renderer.translate(vec3(1,0.5,1));
    renderer.cube();

    renderer.identity();
    renderer.translate(vec3(-1,0.5,0));
    renderer.cylinder();

    renderer.identity();
    renderer.translate(vec3(1,0.5,0));
    renderer.torus();

    renderer.identity();
    renderer.translate(vec3(-1,0.5,1));
    renderer.capsule();
  */
    renderer.identity();
    renderer.translate(vec3(0, 0, 0));
    renderer.scale(vec3(0.25));
    renderer.capsule();
  }
};

int main() {
  MyWindow window;
  window.run();
}
