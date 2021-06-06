#include "agl/window.h"

using namespace glm;
using namespace agl;

class MyWindow : public Window {
  void setup() {
    _tex = renderer.loadTexture("../textures/ParticleCloudWhite.png");
  }

  void draw() {
    renderer.begin(_tex, ADD);
    renderer.quad(vec3(-0.5f, 0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.25f);
    renderer.quad(vec3(0.5f, 0.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f), 0.25f);
    renderer.quad(vec3(0.0f, 0.25f, 0.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.25f);
    renderer.end();
  }

private:
  GLuint _tex;
};

int main() {
  MyWindow window;
  window.run();
}
