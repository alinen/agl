// Copyright 2020, Savvy Sine, Aline Normoyle

#include "agl/window.h"

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

class MyWindow : public agl::Window {
  void draw() {
    // point forward direction of cone towards n
    // and shift base of cone to (0, 0, 0)
    vec3 n(1, 1, 0);
    vec3 z = normalize(n);
    vec3 x = glm::cross(vec3(0, 1, 0), z);
    vec3 y = glm::cross(z, x);

    mat4 T(vec4(x, 0),
           vec4(y, 0),
           vec4(z, 0),
           vec4(0.25, 0.25, 0, 1));

    renderer.transform(T);
    renderer.cone();
  }
};

int main() {
  MyWindow window;
  window.run();
}
