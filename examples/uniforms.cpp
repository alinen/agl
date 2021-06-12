#include "agl/window.h"

class MyWindow : public agl::Window {
  void draw() {
    // Set diffuse color in shaders/phong.fs to red
    renderer.setUniform("Material.diffuse", glm::vec3(1,0,0));
    renderer.sphere();
  }
};

int main() {
  MyWindow window;
  window.run();
}
