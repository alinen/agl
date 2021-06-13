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
    renderer.setUniform("MainTexture.tile", vec2(20.0));

    int halfw = 32;
    int halfh = 32;
    agl::Image checker(halfw*2, halfh*2);
    for (int i = 0; i < checker.height(); i++) {
      for (int j = 0; j < checker.width(); j++) {
        if ((i < halfh && j < halfw) || (i > halfh && j > halfw)) {
          checker.setVec4(i, j, vec4(0.7, 0.7, 0.7, 1));
        } else {
          checker.setVec4(i, j, vec4(1, 1, 1, 1));
        }
      }
    }
    renderer.loadTexture("checker", checker, 0);

    setupPerspectiveScene(vec3(0.0), vec3(5.0));
    background(vec3(0.9f));
  }

  void draw() {
    renderer.setUniform("MainTexture.enabled", true);
    renderer.setUniform("Material.specular", vec3(0.0));
    renderer.setUniform("Material.diffuse", vec3(1));
    renderer.texture("MainTexture.texture", "checker");
    renderer.identity();
    renderer.translate(vec3(0));
    renderer.scale(vec3(20.0f));
    renderer.plane();

    renderer.setUniform("MainTexture.enabled", false);
    renderer.setUniform("Material.specular", vec3(1));
    renderer.setUniform("Material.diffuse", vec3(1, 0, 0));
    renderer.identity();
    renderer.translate(vec3(0, 0.5, 0));
    renderer.scale(vec3(0.5));
    renderer.rotate(elapsedTime(), vec3(0.5, 1, 0));
    renderer.capsule();

    renderer.setUniform("Material.diffuse", vec3(0.5, 1, 0));
    renderer.identity();
    renderer.translate(vec3(-1, 0.5, 1));
    renderer.scale(vec3(0.5));
    renderer.rotate(elapsedTime(), vec3(0.5, 0.5, 0));
    renderer.cone();

    renderer.setUniform("Material.diffuse", vec3(1, 0, 1));
    renderer.identity();
    renderer.translate(vec3(0, 0.5, 1));
    renderer.scale(vec3(0.5));
    renderer.sphere();

    renderer.setUniform("Material.diffuse", vec3(1, 1, 0.3));
    renderer.identity();
    renderer.translate(vec3(1, 0.5, 1));
    renderer.scale(vec3(0.5));
    renderer.rotate(elapsedTime(), vec3(-0.5, 0.5, 0));
    renderer.cube();

    renderer.setUniform("Material.diffuse", vec3(1, 0.8, 0.1));
    renderer.identity();
    renderer.translate(vec3(-1, 0.5, 0));
    renderer.scale(vec3(0.5));
    renderer.rotate(elapsedTime(), vec3(1, 0.5, 0));
    renderer.cylinder();

    renderer.setUniform("Material.diffuse", vec3(1, 0, 0.5));
    renderer.identity();
    renderer.translate(vec3(1, 0.5, 0));
    renderer.scale(vec3(0.75));
    renderer.rotate(elapsedTime(), vec3(0.5, 0.5, 0));
    renderer.teapot();
  }
};

int main() {
  MyWindow window;
  window.run();
}
