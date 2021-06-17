// Copyright 2020, Savvy Sine, Aline Normoyle

#include "agl/window.h"

using glm::vec3;
class MyWindow : public agl::Window {
  void draw() {
    float y = sin(elapsedTime());
    renderer.beginShader("lines");
    renderer.line(vec3(-0.5, y, 0.0), vec3(0.5, -y, 0.0),
      vec3(1, 1, 0), vec3(0, 1, 1));
    renderer.endShader();
  }
};

int main() {
  MyWindow window;
  window.run();
}
