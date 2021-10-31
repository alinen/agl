// Copyright 2021, Savvy Sine, alinen

#include "agl/image.h"

#include <cassert>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace agl {
using glm::vec3;
using glm::vec4;

Image::Image() :
  myData(0),
  myWidth(0),
  myHeight(0),
  myLoaded(false) {
}

Image::Image(int width, int height) :
  myWidth(width), myHeight(height), myLoaded(false) {
  myData = new unsigned char[4*width*height];
}

Image::Image(const Image& orig) {
  myWidth = orig.myWidth;
  myHeight = orig.myHeight;
  myLoaded = false;
  myData = new unsigned char[4 * myWidth * myHeight];
  memcpy(myData, orig.myData, sizeof(unsigned char) * myWidth * myHeight * 4);
}

Image& Image::operator=(const Image& orig) {
  if (&orig == this) {
    return *this;
  }

  clear();

  myWidth = orig.myWidth;
  myHeight = orig.myHeight;
  myLoaded = false;
  myData = new unsigned char[4 * myWidth * myHeight];
  memcpy(myData, orig.myData, sizeof(unsigned char) * myWidth * myHeight * 4);
  return *this;
}

Image::~Image() {
  clear();
}

void Image::set(int width, int height, unsigned char* data) {
  clear();

  myWidth = width;
  myHeight = height;
  myLoaded = false;
  myData = new unsigned char[4 * myWidth * myHeight];
  memcpy(myData, data, sizeof(unsigned char) * myWidth * myHeight * 4);
}

void Image::clear() {
  if (myLoaded) {
    stbi_image_free(myData);
    myData = 0;
    myLoaded = false;
  } else {
    delete[] myData;
  }
}

bool Image::load(const std::string& filename, bool flip) {
  clear();
  stbi_set_flip_vertically_on_load(flip);

  int x, y, n;
  myData = stbi_load(filename.c_str(), &x, &y, &n, 0);
  myWidth = x;
  myHeight = y;
  myLoaded = true;
  return (myData != NULL);
}


bool Image::save(const std::string& filename, bool flip) const {
  stbi_flip_vertically_on_write(flip);
  int result = stbi_write_png(filename.c_str(), myWidth, myHeight,
    4, (unsigned char*) myData, myWidth*4);
  return (result == 1);
}

Pixel Image::get(int row, int col) const {
  assert(row >= 0 && row < myHeight);
  assert(col >= 0 && col < myWidth);

  int idx = (row*myWidth + col) * 4;
  unsigned char r = myData[idx + 0];
  unsigned char g = myData[idx + 1];
  unsigned char b = myData[idx + 2];
  unsigned char a = myData[idx + 3];
  return Pixel{r, g, b, a};
}

void Image::set(int row, int col, const Pixel& color) {
  assert(row >= 0 && row < myHeight);
  assert(col >= 0 && col < myWidth);

  int idx = (row*myWidth + col) * 4;
  myData[idx + 0] = color.r;
  myData[idx + 1] = color.g;
  myData[idx + 2] = color.b;
  myData[idx + 3] = color.a;
}

vec4 Image::getVec4(int i, int j) const {
  assert(i >= 0 && i < myHeight);
  assert(j >= 0 && j < myWidth);

  unsigned int idx = (i * myWidth + j) * 4;
  unsigned char r = myData[idx + 0];
  unsigned char g = myData[idx + 1];
  unsigned char b = myData[idx + 2];
  unsigned char a = myData[idx + 3];
  return glm::vec4(r, g, b, a) / 255.0f;
}

void Image::setVec4(int i, int j, const vec4 & c) {
  assert(i >= 0 && i < myHeight);
  assert(j >= 0 && j < myWidth);

  int idx = (i * myWidth + j) * 4;
  myData[idx + 0] = (unsigned char) (c[0] * 255.999);
  myData[idx + 1] = (unsigned char) (c[1] * 255.999);
  myData[idx + 2] = (unsigned char) (c[2] * 255.999);
  myData[idx + 3] = (unsigned char) (c[3] * 255.999);
}

}  // namespace agl
