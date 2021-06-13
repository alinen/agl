// Copyright 2020, Savvy Sine, Aline Normoyle

#include <vector>
#include "agl/window.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

class MyWindow : public agl::Window {
  void setup() {
    setCameraEnabled(false);  // distance orbit camera
    renderer.loadTexture("particle", "../textures/particle.png", 0);
    renderer.blendMode(agl::ADD);
    _particlePos = vec3(0.0f);
    _lastParticlePos = _particlePos;
  }

  void draw() {
    renderer.beginShader("sprite");
    renderer.texture("image", "particle");
    renderer.sprite(_particlePos, _color, 0.5f);
    renderer.endShader();
  }

  void mouseMotion(int x, int y, int dx, int dy) {
    if (_selected) {
      // convert movement into an amount to change the particle (in world space)
      vec2 screenDir(dx, -dy);
      vec2 ndcDir = (2.0f * screenDir / vec2(width(), height()));

      mat4 projection = renderer.projectionMatrix();
      mat4 view = renderer.viewMatrix();
      vec3 lookfrom = renderer.cameraPosition();

      vec3 pDir = vec3(ndcDir, 0.0);
      mat4 PV = projection * view;
      vec3 delta = vec3(inverse(PV) * vec4(pDir, 0.0));

      _particlePos = _lastParticlePos + delta;
    }
  }

  void mouseDown(int button, int mods) {
    vec2 mousePos = mousePosition();

    // Convert the particle position to screen coords
    mat4 projection = renderer.projectionMatrix();
    mat4 view = renderer.viewMatrix();

    vec4 projectedPos = projection * view * vec4(_particlePos, 1.0);
    vec3 ndcPos = vec3(projectedPos) / projectedPos.w;

    vec2 screenPos = (vec2(ndcPos) + vec2(1.0)) * 0.5f;
    screenPos = screenPos * vec2(width(), height());

    // flip y coordinate because positive Y goes down instead of up
    screenPos.y = height() - screenPos.y;

    if (length(screenPos - mousePos) < _threshold) {
      _color = vec4(1, 1, 0, 1);
      _selected = true;
      _lastParticlePos = _particlePos;
    }
  }

  void mouseUp(int button, int mods) {
    _color = vec4(0, 0, 1, 1);
    _selected = false;
  }

 private:
  vec3 _particlePos;
  vec3 _lastParticlePos;
  vec4 _color = vec4(0, 0, 1, 1);
  bool _selected = false;
  float _threshold = 10;
};

int main() {
  MyWindow window;
  window.run();
}
