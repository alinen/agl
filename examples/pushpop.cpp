// Copyright 2020, Savvy Sine, Aline Normoyle

#include "agl/window.h"

using glm::vec3;

class MyWindow : public agl::Window {
  void draw() {
    renderer.push();
      renderer.translate(vec3(-0.5, 0.0, 0.0));
      renderer.scale(vec3(0.25));
      renderer.sphere();
    renderer.pop();

    renderer.push();
      renderer.translate(vec3(0.5, 0.0, 0.0));
      renderer.scale(vec3(0.75));
      renderer.sphere();
    renderer.pop();
  }
};

int main() {
  MyWindow window;
  window.run();
}
