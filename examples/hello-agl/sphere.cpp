#include "agl/window.h"

class MyWindow : public agl::Window
{
  void draw()
  {
    sphere();
  }
};

int main()
{
  MyWindow window;
  window.run();
}
