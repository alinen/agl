#include "agl/window.h"
#include "agl/mesh/torus.h"

using agl::Torus;
using glm::vec3;

class MyWindow : public agl::Window {
 public:
  void draw() {
    renderer.rotate(elapsedTime(), vec3(1, 1, 0));
    renderer.mesh(_fatTorus);
  }

  Torus _fatTorus = Torus(0.45, 0.5, 20, 20);
};

int main() {
  MyWindow window;
  window.run();
}
