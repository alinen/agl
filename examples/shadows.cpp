// Copyright (c) 2020, Savvy Sine, Aline Normoyle
#include "agl/window.h"

using namespace glm;

class MyWindow : public agl::Window {
  void setup() {
    setWindowSize(1000, 1000);
    renderer.loadShader("shadowmap-1",
      "../shaders/shadowmap-1.vs",
      "../shaders/shadowmap-1.fs");

    renderer.loadShader("shadowmap-2",
      "../shaders/shadowmap-2.vs",
      "../shaders/shadowmap-2.fs");

    renderer.loadDepthTexture("shadowMap", 0, 512, 512);
    renderer.loadTexture("bricks", "../textures/bricks.png", 1);
  }

  void keyDown(int key, int mods) {
    if (key == 'L') animateLight = !animateLight;
    else if (key == '1') lightView = true;
    else if (key == '2') lightView = false;
    else if (key == 'P') usePCF = !usePCF;
    else if (key == 'S') useShadows = !useShadows;
  }

  void drawLight() {
    renderer.beginShader("unlit");
    renderer.setUniform("Material.color", vec4(1.0f));
    renderer.push();
    renderer.translate(vec3(lightpos));
    renderer.scale(vec3(0.2f));
    renderer.sphere();
    renderer.pop();
    renderer.endShader();
  }

  void drawScene(const vec3& campos, const vec3& lookpos) {

    renderer.setUniform("Material.specular", 1.0f, 1.0f, 1.0f);
    renderer.setUniform("Material.diffuse", vec3(0.6f, 0.8f, 1.0f));
    renderer.setUniform("Material.ambient", 0.1f, 0.1f, 0.1f);
    renderer.setUniform("Material.shininess", 80.0f);
    renderer.setUniform("Light.position", lightpos);
    renderer.setUniform("Light.color", 1.0f, 1.0f, 1.0f);
    renderer.texture("mainTexture", "bricks");

    renderer.push();
    renderer.translate(vec3(-1,0.5,0));
    renderer.rotate(glm::radians(45.0f), vec3(1,0,1));
    renderer.torus();
    renderer.pop();

    renderer.push();
    renderer.translate(vec3(1,1,0));
    renderer.scale(vec3(1,2,1));
    renderer.cube();
    renderer.pop();

    renderer.push();
    renderer.scale(vec3(20, 0.1, 20));
    renderer.cube();
    renderer.pop();
  }

  void draw() {
    float aspect = ((float)width()) / height();

    if (animateLight) {
      lightpos.x = 3*cos(elapsedTime());
      lightpos.z = 3*sin(elapsedTime());
    }

    mat4 shadowMatrix = mat4(
      vec4(0.5, 0.0, 0.0, 0.0),
      vec4(0.0, 0.5, 0.0, 0.0),
      vec4(0.0, 0.0, 0.5, 0.0),
      vec4(0.5, 0.5, 0.5, 1.0));

    // render pass #1
    if (!lightView) renderer.beginRenderTexture("shadowMap");
    renderer.perspective(glm::radians(60.0f), aspect, 0.5f, 50.0f);
    renderer.cullMode(agl::FRONT);
    renderer.beginShader("shadowmap-1");
    background(vec3(0));
    renderer.lookAt(lightpos, lookPos, up);
    shadowMatrix = shadowMatrix * 
      renderer.projectionMatrix() *
      renderer.viewMatrix();
    drawScene(lightpos, lookPos);
    renderer.endShader();
    if (!lightView) renderer.endRenderTexture();

    // render pass #2
    if (!lightView) {
      background(vec3(0));
      renderer.perspective(glm::radians(60.0f), aspect, 0.5f, 50.0f);
      renderer.cullMode(agl::BACK);
      // update camera
      eyePos[0] = radius * cos(azimuth) * cos(elevation);
      eyePos[1] = radius * sin(elevation);
      eyePos[2] = radius * sin(azimuth) * cos(elevation);
      renderer.lookAt(eyePos, lookPos, up);
      drawLight();
      renderer.beginShader("shadowmap-2");
      renderer.setUniform("ShadowMatrix", shadowMatrix);
      renderer.setUniform("PCF", usePCF);
      renderer.setUniform("useShadows", useShadows);
      drawScene(eyePos, lookPos);
      renderer.endShader();
    }
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
  bool animateLight = false;
  bool lightView = false;
  bool usePCF = false;
  bool useShadows = true;
};

int main() {
  MyWindow window;
  window.run();
}
