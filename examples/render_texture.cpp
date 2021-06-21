// Copyright 2020, Savvy Sine, Aline Normoyle

#include "agl/window.h"

using glm::vec3;
class MyWindow : public agl::Window {
  void setup() {
    renderer.loadRenderTexture("teapotTex", 0, 512, 512);
  }

  void draw() {
    renderer.beginRenderTexture("teapotTex");
    background(vec3(0.5, 1.0, 0.0));
    renderer.setUniform("MainTexture.enabled", false);
    renderer.scale(vec3(sin(elapsedTime())));
    renderer.teapot();
    renderer.endRenderTexture();

    background(vec3(0.3));
    renderer.setUniform("MainTexture.enabled", true);
    renderer.texture("MainTexture.texture", "teapotTex");
    renderer.identity();
    renderer.rotate(elapsedTime(), vec3(1, 1, 0));
    renderer.cube();
  }
};

int main() {
  MyWindow window;
  window.run();
}
