// Copyright 2020, Aline Normoyle, alinen@savvysine.com, MIT License

#include "agl/window.h"
#include <iostream>

using glm::vec3;

class MyWindow : public agl::Window {
  void setup() {
    setCameraEnabled(false);
    background(glm::vec3(0.3f, 0.3f, 0.32f));
    std::cout << width() << " " << height() << std::endl;
  }

  void draw() {
    
    renderer.ortho(0, width(), height(), 0, -100, 100);

    renderer.push();
    renderer.translate(glm::vec3(250, 250, 0));
    renderer.scale(glm::vec3(100));
    renderer.sphere();
    renderer.pop();

    renderer.fontColor(glm::vec4(0.5, 0, 0, 0.5));
    renderer.text("SPHERE!", 250, 250);
  }
};

int main() {
  MyWindow window;
  window.run();
}
