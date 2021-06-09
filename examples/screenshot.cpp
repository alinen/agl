#include "agl/window.h"

class MyWindow : public agl::Window {
  void setup() {
    renderer.sphere();
    screenshot("sphere.png");
  }
};

int main() {
  MyWindow window;
  window.run();
}
