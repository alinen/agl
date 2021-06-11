#include "agl/window.h"

class MyWindow : public agl::Window {
  void draw() {
    // Set main color in shaders/phong.fs
    renderer.setUniform("Material.Kd", glm::vec3(1,0,0));
    renderer.sphere();
  }
};

int main() {
  MyWindow window;
  window.run();
}
