#include "agl/window.h"

class MyWindow : public agl::Window {
  void draw() {
    // Set diffuse color in shaders/phong.fs to blue
    renderer.setUniform("Material.diffuse", 0.4f, 0.6f, 1.0f);
    renderer.sphere();
  }
};

int main() {
  MyWindow window;
  window.run();
}
