// Copyright 2020, Savvy Sine, Aline Normoyle

#include "agl/window.h"

class MyWindow : public agl::Window {
  void draw() {
    float x = sin(elapsedTime());
    renderer.translate(glm::vec3(x, 0.0, 0.0));
    renderer.sphere();
  }
};

int main() {
  MyWindow window;
  window.run();
}
