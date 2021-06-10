#include "agl/window.h"

class MyWindow : public agl::Window {
  void draw() {
    renderer.scale(glm::vec3(0.75f, 0.5f, 1.0f));
    renderer.sphere();
  }
};

int main() {
  MyWindow window;
  window.run();
}
