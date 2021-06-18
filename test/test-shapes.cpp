// Copyright (c) 2020, Savvy Sine, Aline Normoyle
#include "agl/window.h"
#include "agl/renderer.h"

using glm::vec3;
class MyWindow : public agl::Window {
  void setup() {
    background(vec3(0));
    renderer.identity();
    renderer.sphere();
    screenshot("test-sphere.png");

    background(vec3(0));
    renderer.identity();
    renderer.cube();
    screenshot("test-cube.png");

    background(vec3(0));
    renderer.identity();
    renderer.cylinder();
    screenshot("test-cylinder.png");

    noLoop();
  }
};

int main() {
  MyWindow window;
  window.run(); 
}
