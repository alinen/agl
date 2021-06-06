#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace agl;
using namespace glm;

class MyWindow : public Window
{
  void setup() {
    renderer.loadCubemap("cubemap", "../textures/sky", 0);
  }  
  void draw()
  {
    renderer.beginShader("cubemap");
    renderer.skybox();
    renderer.endShader();
  }
};

int main()
{
  MyWindow window;
  window.run();
}
