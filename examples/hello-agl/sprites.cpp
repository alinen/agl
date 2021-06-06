#include "agl/window.h"

using namespace glm;
using namespace agl;

class MyWindow : public Window {
  void setup() {
    renderer.loadTexture("cloud", "../textures/ParticleCloudWhite.png", 0);
    renderer.loadTexture("particle", "../textures/particle.png", 0);
    renderer.blendMode(ADD);
  }

  void draw() {
    renderer.beginShader("sprite");
    renderer.texture("image", "cloud");
    renderer.quad(vec3(-0.5f, 0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.25f);
    renderer.quad(vec3(0.5f, 0.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f), 0.25f);

    renderer.texture("image", "particle");
    renderer.quad(vec3(0.0f, 0.25f, 0.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.25f);
    renderer.endShader();
  }
};

int main() {
  MyWindow window;
  window.run();
}
