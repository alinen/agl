// Aline Normoyle, 2023
#include "agl/window.h"

using glm::vec3;

class MyWindow : public agl::Window {
  void setup() {
    setWindowSize(1000, 1000);
  }
  void draw() {
    
    float theta = elapsedTime();
    float px = _radiusPlanet * cos(theta);
    float py = _radiusPlanet * sin(theta);

    float mx = _radiusMoon * cos(theta * _deltaMoon);
    float my = _radiusMoon * sin(theta * _deltaMoon);
    renderer.push();
      renderer.translate(vec3(px, py, 0.0));

      renderer.push();
        renderer.translate(vec3(mx, my, 0.0));
        renderer.scale(vec3(0.05));
        renderer.sphere();
      renderer.pop();

      renderer.scale(vec3(0.1));
      renderer.sphere();
    renderer.pop();

    renderer.push();
    renderer.scale(vec3(0.2));
    renderer.sphere();
    renderer.pop();
  }
private:
  float _deltaPlanet = 0.5f;
  float _deltaMoon = 2.0f;
  float _radiusPlanet = 0.5f;
  float _radiusMoon = 0.1f;
};

int main() {
  MyWindow window;
  window.run();
}
