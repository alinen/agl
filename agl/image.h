// Copyright 2021, savvy_sine, alinen
// Class for loading, modifying, and saving RGBA images 

#ifndef AGL_IMAGE_H_
#define AGL_IMAGE_H_

#include <iostream>
#include <string>
#include "agl/aglm.h"

namespace agl {
struct Pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

class Image {
 public:
  Image();
  Image(int width, int height);
  Image(const Image& orig);
  Image& operator=(const Image& orig);

  virtual ~Image();

  // load the given filename
  bool load(const std::string& filename);

  // save the given filename
  bool save(const std::string& filename, bool flip = true) const;

  // return the current width
  inline int width() const { return myWidth; }

  // return the current height
  inline int height() const { return myHeight; }

  // return the data
  inline unsigned char* data() const { return myData; }

  // Replace image RGBA data
  void set(int width, int height, unsigned char* data);

  // Get the pixel at index (row, col)
  Pixel get(int row, int col) const;

  // Get the pixel at index (row, col)
  void set(int row, int col, const Pixel& color);

  // Set a color with components in range [0, 1]
  void setVec4(int row, int col, const glm::vec4& color);

  // Get a vec3 color
  glm::vec4 getVec4(int row, int col) const;

 private:
  void clear();

 private:
  unsigned char* myData;
  unsigned int myWidth;
  unsigned int myHeight;
  bool myLoaded;
};
}  // namespace agl
#endif  // AGL_IMAGE_H_
