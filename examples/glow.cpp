// Copyright (c) 2020, Savvy Sine, Aline Normoyle
#include "agl/window.h"

using namespace glm;

class MyWindow : public agl::Window {
  void setup() {
    setWindowSize(1000, 1000);

    renderer.loadShader("screeneffects",
      "../shaders/shadertoy.vs",
      "../shaders/screeneffects.fs");

    renderer.loadRenderTexture("SceneRender", 0, 512, 512);
  }

  void keyDown(int key, int mods) {
    if (key == '0') effect = 0;
    else if (key == '1') effect = 1;
    else if (key == '2') effect = 2;
    else if (key == '3') effect = 3;
    else if (key == '4') effect = 4;
    else if (key == '5') effect = 5;
    else if (key == '6') effect = 6;
  }

  void drawScene(const vec3& campos, const vec3& lookpos) {

    renderer.push();
    renderer.translate(vec3(-1, 0.5, 0));
    renderer.rotate(glm::radians(45.0f), vec3(1, 0, 1));
    renderer.torus();
    renderer.pop();

    renderer.push();
    renderer.translate(vec3(1, 1, 0));
    renderer.scale(vec3(1, 2, 1));
    renderer.cube();
    renderer.pop();

    renderer.push();
    renderer.scale(vec3(20, 0.1, 20));
    renderer.cube();
    renderer.pop();
  }

  void draw() {
    float aspect = ((float)width()) / height();

    // render pass #1
    renderer.beginRenderTexture("SceneRender");
    renderer.beginShader("phong");
    renderer.setUniform("Material.specular", 1.0f, 1.0f, 1.0f);
    renderer.setUniform("Material.diffuse", vec3(0.6f, 0.8f, 1.0f));
    renderer.setUniform("Material.ambient", 0.1f, 0.1f, 0.1f);
    renderer.setUniform("Material.shininess", 80.0f);
    renderer.setUniform("Light.position", lightpos);
    renderer.setUniform("Light.color", 1.0f, 1.0f, 1.0f);

    background(vec3(0));
    renderer.perspective(glm::radians(60.0f), aspect, 0.5f, 50.0f);

    // update camera
    eyePos[0] = radius * cos(azimuth) * cos(elevation);
    eyePos[1] = radius * sin(elevation);
    eyePos[2] = radius * sin(azimuth) * cos(elevation);
    renderer.lookAt(eyePos, lookPos, up);
    drawScene(eyePos, lookPos);
    renderer.endShader();
    renderer.endRenderTexture();

    // render pass #2
    renderer.beginShader("screeneffects");
    lookAt(vec3(0), vec3(0, 0, -2));
    ortho(0, width(), 0, height(), -1, 1);
    background(vec3(0));
    renderer.setUniform("EffectType", effect);
    renderer.setUniform("GlobalTime", elapsedTime());
    renderer.setUniform("Resolution", vec2(width(), height()));
    renderer.texture("ScreenTexture", "SceneRender");
    renderer.identity();
    renderer.translate(vec3(width()*0.5, height()*0.5, 0.0));
    renderer.scale(vec3(width(), height(), 1.0f));
    renderer.rotate(kPI / 2, vec3(1, 0, 0));
    renderer.plane();
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

  vec4 lightpos = vec4(3, 4, 3, 1);

  vec3 eyePos = vec3(0, 0, 3);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);

  float elevation = 0.2;
  float azimuth = 1.3;
  float radius = 5;
  bool mouseIsDown = false;
  float speed = 0.01f;
  int effect = 0;
};

int main() {
  MyWindow window;
  window.run();
}
