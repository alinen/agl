// Copyright 2020, Savvy Sine, Aline Normoyle

#include "agl/window.h"
#include "agl/mesh/plane.h"

using glm::vec2;
using glm::vec4;
using glm::vec3;

class UndulateMesh : public agl::Plane {
 public:
  UndulateMesh(int xsize, int ysize) : Plane(1, 1, xsize, ysize) {
    setIsDynamic(true);
    init();  // initialize the mesh rather than wait for first frame
  }

  void update(float elapsedTime) {
    for (int i = 0; i < numVertices(); i++) {
      vec3 p = vec3(vertexData(POSITION, i));
      setVertexData(POSITION, i, vec4(position(p, elapsedTime), 0));
      setVertexData(NORMAL, i, vec4(normal(p, elapsedTime), 0));
    }
  }

  vec3 position(const vec3& p, float t) {
    float angle = t;
    float frequency = 7.0;
    float amplitude = 0.05;

    float heightFn = (angle + frequency * p[0] * frequency * p[2]);
    float y = amplitude * sin(heightFn);
    return vec3(p.x, y, p.z);
  }

  vec3 normal(const vec3& p, float t) {
    float eps = 0.001;
    vec3 x = position(p+vec3(eps, 0, 0), t) - position(p-vec3(eps, 0, 0), t);
    vec3 z = position(p+vec3(0, 0, eps), t) - position(p-vec3(0, 0, eps), t);
    vec3 y = glm::cross(z, x);
    return normalize(y);
  }
};

class MyWindow : public agl::Window {
 public:
  void setup() {
    perspective(glm::radians(30.0), 1, 0.1, 100);
    renderer.setUniform("Material.specular", vec3(1.0, 0.2, 0.8));
    renderer.setUniform("Material.ambient", vec3(0.3, 0.0, 0.2));
  }

  void draw() {
    _mesh.update(elapsedTime());
    renderer.rotate(kPI * 0.2, vec3(1, 0, 0));
    renderer.mesh(_mesh);
  }

  UndulateMesh _mesh = UndulateMesh(100, 100);
};

int main() {
  MyWindow window;
  window.run();
}
