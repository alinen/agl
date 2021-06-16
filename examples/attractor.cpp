// Copyright 2020, Savvy Sine, Aline Normoyle
// Visualizes strange attractors 
// See: Julien C. Sprott, 2000, Strange Attractors: Creating patterns in Chaos

#include "agl/window.h"
#include "agl/point_mesh.h"

using glm::vec2;
using glm::vec4;
using glm::vec3;

class Attractor : public agl::PointMesh {
 public:
  Attractor() : PointMesh() {
    init();  // initialize the mesh rather than wait for first frame
  }

  void setParameters(const std::string& name, std::vector<float>* params) {
    float startc = 32;
    float startv = -4.5;
    params->clear();
    for (int i = 0; i < name.size(); i++) {
      char c = name[i];
      float v = (c - startc)*0.1 + startv;
      std::cout << i << ") " << name[i] << " = " << v << "\n";
      params->push_back(v);
    }
  }

  void init() override {
    std::vector<float> a;
    //setParameters("MTISVBKHOIJFWSYEKEGYLWJKEOGVLM", &a);
    setParameters("JKRADSXGDBHIJTQJJDICEJKYSTXFNU", &a);

    int iterationsMax = 400000;
    float minValue = -1000000;
    float maxValue =  1000000;
    vec3 maxBound = vec3(-10.1);  // for window bounds
    vec3 minBound = vec3(10.1);   // for window bounds
    bool done = false;
    vec3 p = vec3(0.05);

    std::vector<GLfloat> points;
    for (int i = 0; i < iterationsMax && !done; i++) {
      float x = p.x;
      float y = p.y;
      float z = p.z;

      float xnew = a[0] + a[1]*x + a[2]*x*x + a[3]*x*y + a[4]*x*z + 
                   a[5]*y + a[6]*y*y + a[7]*y*z + a[8]*z + a[9]*z*z;

      float ynew = a[10] + a[11]*x + a[12]*x*x + a[13]*x*y + a[14]*x*z + 
                   a[15]*y + a[16]*y*y + a[17]*y*z + a[18]*z + a[19]*z*z;

      float znew = a[20] + a[21]*x + a[22]*x*x + a[23]*x*y + a[24]*x*z + 
                   a[25]*y + a[26]*y*y + a[27]*y*z + a[28]*z + a[29]*z*z;

      p = glm::clamp(vec3(xnew, ynew, znew), vec3(minValue), vec3(maxValue));
      if (std::abs(p.x) > maxValue-1) {
        done = true;

      } else {
        points.push_back(p.x);
        points.push_back(p.y);
        points.push_back(p.z);
      }

      maxBound = glm::max(p, maxBound); 
      minBound = glm::min(p, minBound); 
    }
    bounds = maxBound - minBound;
    center = minBound + 0.5f * (maxBound - minBound);
    //std::cout << bounds << " " << center << std::endl;
    initBuffers(&points, 0, 0, 0);
  }

 public:
  vec3 bounds = vec3(0);
  vec3 center = vec3(0.0);
};

class MyWindow : public agl::Window {
 public:
  void setup() {
    background(vec3(1.0));

    float fov = glm::radians(45.0);
    vec3 bounds = _mesh.bounds;
    float minD = std::min(std::min(bounds.x, bounds.y), bounds.z);
    float maxD = std::max(std::max(bounds.x, bounds.y), bounds.z);
    float near = minD * 0.05f / tan(fov);
    float far = maxD * 10.0f;

    perspective(fov, width()/height(), near, far);
    lookAt(vec3(0,0,-2), vec3(0));
  }

  void draw() {
    renderer.beginShader("unlit");
    renderer.setUniform("Material.color", vec4(0.25, 0.25, 0.25, 1));
    renderer.translate(-_mesh.center);
    renderer.mesh(_mesh);
    renderer.endShader();
  }

  Attractor _mesh;
};

int main() {
  MyWindow window;
  window.run();
}
