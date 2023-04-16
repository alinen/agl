// Copyright 2020, Savvy Sine, Aline Normoyle

#include "agl/window.h"
#include "agl/mesh/plane.h"

using glm::vec2;
using glm::vec4;
using glm::vec3;

#define LERP(a,b,t) ((1-t)*(a) + t*(b))
#define CLAMP(a, m) ((a) >= m? (m-1) : (a))

class HeightmapMesh : public agl::Plane {
 public:
   HeightmapMesh(int xsize, int ysize) : Plane(1, 1, xsize, ysize) {
     setIsDynamic(true);
   }

   virtual void init(const agl::Image& hm) {
    agl::Plane::init();  // initialize the mesh rather than wait for first frame


    for (int i = 0; i < numVertices(); i++) {
      vec3 p = vec3(vertexData(POSITION, i));
      float cellsizex = _xsize / _xdivs;
      float cellsizez = _zsize / _zdivs;

      vec3 p2 = heightPos(p.x+cellsizex, p.z, hm);
      vec3 p1 = heightPos(p.x, p.z, hm);
      vec3 p0 = heightPos(p.x, p.z + cellsizez, hm);
      vec3 normal = normalize(cross(p0 - p1, p2 - p1));

      setVertexData(POSITION, i, vec4(p1, 0));
      setVertexData(NORMAL, i, vec4(normal, 0));
    }
  }

  vec3 heightPos(float x, float z, const agl::Image& hm) {
    float halfw = _xsize * 0.5f;
    float halfh = _zsize * 0.5f;
    int width = hm.width();
    int height = hm.height();
    float u = width * (x + halfw) / _xsize;
    float v = height * (z + halfh) / _zsize;
    float h = altitude(u, v, hm);
    return vec3(x, h, z);
  }

  float altitude(float u, float v, const agl::Image& hm) {
    float h = hm.height();
    float w = hm.width();
    float i = CLAMP((int)v, h); // up/down
    float j = CLAMP((int)u, w); // left/right
    float ifrac = std::max(v - i, 0.0f);
    float jfrac = std::max(u - j, 0.0f);

    float hi_bl = hm.get(i, j).r;
    float hi_br = hm.get(CLAMP(i+1, h), j).r;
    float hi_tl = hm.get(i, CLAMP(j+1, w)).r;
    float hi_tr = hm.get(CLAMP(i+1, h), CLAMP(j + 1, w)).r;

    float hi1 = LERP(hi_bl, hi_br, ifrac);
    float hi2 = LERP(hi_tl, hi_tr, ifrac);
    float height = LERP(hi1, hi2, jfrac)/255.0f;
    return height;
  }
};

class MyWindow : public agl::Window {
 public:
  void setup() {
    perspective(glm::radians(60.0), 1, 0.1, 1000);
    renderer.setUniform("Material.specular", vec3(1.0, 0.2, 0.8));
    renderer.setUniform("Material.ambient", vec3(0.3, 0.0, 0.2));

    agl::Image heightmap;
    heightmap.load("../textures/heightfield.tga");
    _mesh.init(heightmap);
  }

  void draw() {
    renderer.scale(vec3(10, 1, 10));
    renderer.mesh(_mesh);
  }

  HeightmapMesh _mesh = HeightmapMesh(100, 100);
};

int main() {
  MyWindow window;
  window.run();
}
