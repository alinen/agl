// Copyright 2020, Savvy Sine, Aline Normoyle

#include "agl/window.h"

using glm::vec3;

class MyWindow : public agl::Window {
  void setup() {
    renderer.loadTexture("cloud", "../textures/cloud.png", 0);
    renderer.loadTexture("particle", "../textures/particle.png", 0);
    renderer.blendMode(agl::ADD);
  }

  void draw() {
    renderer.beginShader("sprite");
    renderer.texture("image", "cloud");
    renderer.sprite(vec3(-0.5f, 0.0f, 0.0f), red, 0.25f);
    renderer.sprite(vec3(0.5f, 0.0f, 0.0f), green, 0.25f);

    renderer.texture("image", "particle");
    renderer.sprite(vec3(0.0f, 0.25f, 0.0f), blue, 0.25f);
    renderer.endShader();
  }

  const glm::vec4 red = glm::vec4(1, 0, 0, 1);
  const glm::vec4 green = glm::vec4(0, 1, 0, 1);
  const glm::vec4 blue = glm::vec4(0, 0, 1, 1);
};

int main() {
  MyWindow window;
  window.run();
}
