#include "agl/window.h"

class MyWindow : public agl::Window
{
  void draw()
  {
    renderer.sphere();
  }
};

int main()
{
  MyWindow window;
  window.run();
}
