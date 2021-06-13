// Copyright 2020, Savvy Sine, Aline Normoyle

#include "agl/window.h"

class MyWindow : public agl::Window {
  void draw() {
    float angleRad = elapsedTime();  // angle is in radians
    renderer.rotate(angleRad, glm::vec3(0.707f, 0.707f, 0.0f));
    renderer.cube();
  }
};

int main() {
  MyWindow window;
  window.run();
}
