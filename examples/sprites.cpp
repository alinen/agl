#include "agl/window.h"

using namespace glm;
using namespace agl;

class MyWindow : public Window {
  void setup() {
    renderer.loadTexture("cloud", "../textures/cloud.png", 0);
    renderer.loadTexture("particle", "../textures/particle.png", 0);
    renderer.blendMode(ADD);
  }

  void draw() {
    renderer.beginShader("sprite");
    renderer.texture("image", "cloud");
    renderer.sprite(vec3(-0.5f, 0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.25f);
    renderer.sprite(vec3(0.5f, 0.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f), 0.25f);

    renderer.texture("image", "particle");
    renderer.sprite(vec3(0.0f, 0.25f, 0.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.25f);
    renderer.endShader();
  }
};

int main() {
  MyWindow window;
  window.run();
}
