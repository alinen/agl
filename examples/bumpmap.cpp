// Copyright (c) 2020, Savvy Sine, Aline Normoyle
#include "agl/window.h"

using namespace glm;

class MyWindow : public agl::Window {
  void setup() {
    setWindowSize(1000, 1000);
    renderer.loadShader("bumpmap", 
      "../shaders/bumpmap.vs",
      "../shaders/bumpmap.fs");

    renderer.loadTexture("bricks", "../textures/bricks.png", 0);
    renderer.loadTexture("bricks-normal", "../textures/bricks-normal-map.png", 1);
    perspective(glm::radians<float>(60.0f), 1.0f, 0.1f, 100.0f);
  }

  void keyDown(int key, int mods) {
    if (key == 'N') useNormalMap = !useNormalMap;
    else if (key == 'L') animateLight = !animateLight;
  }

  void draw() {
    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    // update camera
    eyePos[0] = radius * cos(azimuth) * cos(elevation);
    eyePos[1] = radius * sin(elevation);
    eyePos[2] = radius * sin(azimuth) * cos(elevation);
    renderer.lookAt(eyePos, lookPos, up);

    renderer.beginShader("lines");
    renderer.line(vec3(0), vec3(1, 0, 0), vec3(1, 0, 0), vec3(1, 0, 0));
    renderer.line(vec3(0), vec3(0, 1, 0), vec3(0, 1, 0), vec3(0, 1, 0));
    renderer.line(vec3(0), vec3(0, 0, 1), vec3(0, 0, 1), vec3(0, 0, 1));
    renderer.endShader();

    if (animateLight) {
      lightpos.x = cos(elapsedTime()*2);
      lightpos.y = sin(elapsedTime()*2);
    }
    renderer.beginShader("unlit");
    renderer.setUniform("Material.color", vec4(1.0f));
    renderer.push();
    renderer.translate(vec3(lightpos));
    renderer.scale(vec3(0.2f));
    renderer.sphere();
    renderer.pop();
    renderer.endShader();

    renderer.beginShader("bumpmap");
    renderer.setUniform("Material.specular", 1.0f, 1.0f, 1.0f);
    renderer.setUniform("Material.diffuse", vec3(0.6f, 0.8f, 1.0f));
    renderer.setUniform("Material.ambient", 0.1f, 0.1f, 0.1f);
    renderer.setUniform("Material.shininess", 80.0f);
    renderer.setUniform("Light.position", lightpos);
    renderer.setUniform("Light.color", 1.0f, 1.0f, 1.0f);
    renderer.setUniform("useNormalMap", useNormalMap);

    renderer.texture("texture", "bricks");
    renderer.texture("normalmap", "bricks-normal");
    renderer.cube();
    renderer.endShader();
  }

  void mouseMotion(int x, int y, int dx, int dy) {
    if (!mouseIsDown) return;
    if (fabs(dx) > fabs(dy)) {
      azimuth += dx * speed;
    }
    else {
      elevation += dy * speed;
      elevation = std::min(elevation, glm::radians(89.0f));
      elevation = std::max(elevation, glm::radians(-89.0f));
    }
  }

  void mouseDown(int button, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      mouseIsDown = true;
    }
  }

  void mouseUp(int button, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      mouseIsDown = false;
    }
  }

  void scroll(float dx, float dy) {
    radius += dy;
  }

  vec4 lightpos = vec4(1, 0, 3, 1);

  vec3 eyePos = vec3(0, 0, 3);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);

  float elevation = 0;
  float azimuth = 0;
  float radius = 3;
  bool mouseIsDown = false;
  float speed = 0.01f;
  bool useNormalMap = true;
  bool animateLight = false;
};

int main() {
  MyWindow window;
  window.run();
}
