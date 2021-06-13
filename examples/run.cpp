// Copyright 2020, Savvy Sine, Aline Normoyle

#include "agl/window.h"

class MyWindow : public agl::Window {
  void setup() {
    setWindowSize(200, 200);
  }

  void draw() {
    renderer.sphere();
  }
};

int main() {
  MyWindow window;
  window.run();
}
