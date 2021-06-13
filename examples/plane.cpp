#include "agl/window.h"

using glm::vec2;
using glm::vec3;

class MyWindow : public agl::Window {
  void setup() {
    renderer.setUniform("Fog.enabled", true);
    renderer.setUniform("Fog.color", vec3(0.9f));
    renderer.setUniform("Fog.minDist", 5.0f);
    renderer.setUniform("Fog.maxDist", 9.0f);
    renderer.setUniform("Material.specular", vec3(0.0));
    renderer.setUniform("MainTexture.enabled", true);
    renderer.setUniform("MainTexture.tile", vec2(10.0));

    renderer.loadTexture("bricks", "../textures/bricks.png", 0);
    renderer.texture("MainTexture.texture", "bricks");

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
