// Copyright 2020, Savvy Sine, Aline Normoyle

#include "agl/window.h"
#include "agl/image.h"

using glm::vec2;
using glm::vec3;

class MyWindow : public agl::Window {
  void setup() {
    renderer.setUniform("Material.specular", vec3(0.0));
    renderer.setUniform("MainTexture.enabled", true);
    renderer.setUniform("MainTexture.tile", vec2(10.0));

    _texture.load("../textures/bricks.png");
    renderer.loadTexture("bricks", _texture, 0);
    renderer.texture("MainTexture.texture", "bricks");

    setupPerspectiveScene(vec3(0.0), vec3(10.0));
    background(vec3(0.9f));
  }

  void draw() {
    renderer.scale(vec3(20.0f));
    renderer.plane();
  }

  void keyUp(int key, int mods) {
    if (key == GLFW_KEY_UP) {
      swirl();
      renderer.loadTexture("bricks", _texture, 0);  // replace old image
    }
  }

  void swirl() {
    for (int i = 0; i < _texture.height(); i++) {
      for (int j = 0; j < _texture.width(); j++) {
        agl::Pixel color = _texture.get(i, j);

        // swirl colors
        unsigned char red = color.r;
        color.r = color.g;
        color.g = color.b;
        color.b = red;

        _texture.set(i, j, color);
      }
    }
  }

 private:
  agl::Image _texture;
};

int main() {
  MyWindow window;
  window.run();
}
