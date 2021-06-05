// alinen, 2021
#include "image.h"
#include <cassert>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using namespace agl;
using namespace std;
using namespace glm;
      
Image::Image() : myData(0), myWidth(0), myHeight(0), myLoaded(false)
{
   stbi_set_flip_vertically_on_load(true);
}

Image::Image(int width, int height) : myWidth(width), myHeight(height), myLoaded(false)
{
    myData = new Pixel[width*height];
}

Image::Image(const Image& orig)
{
    cout << "not implemented\n";
}

Image& Image::operator=(const Image& orig)
{
    if (&orig == this)
    {
        return *this;
    }

    cout << "not implemented\n";
    return *this;
}

Image::~Image()
{
   clear();
}

void Image::clear()
{
   if (myLoaded)
   {
      stbi_image_free(myData);
      myData = 0;
      myLoaded = false;
   }
   else
   {
      delete[] myData;
   }
}

bool Image::load(const std::string& filename) 
{
   clear();

   int x, y, n;
   myData = (Pixel*) stbi_load(filename.c_str(), &x, &y, &n, 0);
   myWidth = x;
   myHeight = y;
   myLoaded = true;
   return (myData != NULL);
}


bool Image::save(const std::string& filename) const
{
    int result = stbi_write_png(filename.c_str(), myWidth, myHeight, 
        3, (unsigned char*) myData, myWidth*3);
    return (result == 1);
}

Pixel Image::get(int row, int col) const
{
    assert(row >= 0 && row < myHeight);
    assert(col >= 0 && col < myWidth);
    return myData[row*myWidth + col];
}

void Image::set(int row, int col, const Pixel& color)
{
    assert(row >= 0 && row < myHeight);
    assert(col >= 0 && col < myWidth);
    myData[row*myWidth + col] = color;
}

vec3 Image::get_vec3(int i, int j) const
{
   assert(i >= 0 && i < myHeight);
   assert(j >= 0 && j < myWidth);

   unsigned int idx = i * myWidth + j;
   Pixel p = myData[idx];
   return glm::vec3(p.r / 255.0, p.g / 255.0, p.b / 255.0);
}

void Image::set_vec3(int i, int j, const vec3 & c)
{
   assert(i >= 0 && i < myHeight);
   assert(j >= 0 && j < myWidth);

   int idx = i * myWidth + j;
   myData[idx].r = (unsigned char) (c[0] * 255.999);
   myData[idx].g = (unsigned char) (c[1] * 255.999);
   myData[idx].b = (unsigned char) (c[2] * 255.999);
}
