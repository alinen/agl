#include "agl/window.h"
#include "agl/image.h"

using namespace glm;
using namespace agl;

class MyWindow : public Window {
  void setup() {
    Image img;
    img.load("../textures/ParticleCloudWhite.png");
    renderer.loadTexture("cloud", img, 0);
    renderer.blendMode(ADD);
  }

  void draw() {
    renderer.beginShader("sprite");
    renderer.texture("image", "cloud");
    renderer.quad(vec3(-0.5f, 0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.25f);
    renderer.quad(vec3(0.5f, 0.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f), 0.25f);
    renderer.quad(vec3(0.0f, 0.25f, 0.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.25f);
    renderer.endShader();
  }
};

int main() {
  MyWindow window;
  window.run();
}
