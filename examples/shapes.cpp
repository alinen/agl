#include "agl/window.h"

using glm::vec3;

class MyWindow : public agl::Window {
  void setup() {
    renderer.perspective(glm::radians(30.0f), 1.0f, 0.1f, 100.0f);
    renderer.lookAt(vec3(0,0,5), vec3(0,0,0));
  }

  void draw() {
    //renderer.identity();
    //renderer.translate(vec3(0));
    //renderer.plane(); 

/*
    renderer.identity();
    renderer.translate(vec3(-1,0.5,1));
    renderer.cone();

    renderer.identity();
    renderer.translate(vec3(0,0.5,1));
    renderer.sphere();

    renderer.identity();
    renderer.translate(vec3(1,0.5,1));
    renderer.cube();

    renderer.identity();
    renderer.translate(vec3(-1,0.5,0));
    renderer.cylinder();

    renderer.identity();
    renderer.translate(vec3(1,0.5,0));
    renderer.torus();

    renderer.identity();
    renderer.translate(vec3(-1,0.5,1));
    renderer.capsule();
  */
    renderer.identity();
    renderer.translate(vec3(0,0,0));
    renderer.capsule();
  }
};

int main() {
  MyWindow window;
  window.run();
}
