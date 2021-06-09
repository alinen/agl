#include "agl/window.h"

class MyWindow : public agl::Window {
  void setup() {
    setSize(200,200);
  }

  void draw() {
    renderer.sphere();
  }
};

int main() {
  MyWindow window;
  window.run();
}
