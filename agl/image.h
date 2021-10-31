// Copyright 2021, Savvy Sine, alinen

#ifndef AGL_IMAGE_H_
#define AGL_IMAGE_H_

#include <iostream>
#include <string>
#include "agl/aglm.h"

namespace agl {

/**
 * @brief Holder for a RGBA color
 * 
 * @see Image
 */
struct Pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

/**
 * @brief Implements loading, modifying, and saving RGBA images
 */
class Image {
 public:
  Image();
  Image(int width, int height);
  Image(const Image& orig);
  Image& operator=(const Image& orig);

  virtual ~Image();

  /** 
   * @brief Load the given filename (.png)
   * @param filename The file to load, relative to the running directory
   * @param flip Whether the file should flipped vertally when loaded
   * 
   * @verbinclude sprites.cpp
   */
  bool load(const std::string& filename, bool flip = false);

  /** 
   * @brief Save the image to the given filename (.png)
   * @param filename The file to load, relative to the running directory
   * @param flip Whether the file should flipped vertally before being saved
   */
  bool save(const std::string& filename, bool flip = true) const;

  /** @brief Return the image width in pixels
   */
  inline int width() const { return myWidth; }

  /** @brief Return the image height in pixels
   */
  inline int height() const { return myHeight; }

  /** 
   * @brief Return the RGBA data
   *
   * Data will have size width * height * 4 (RGBA)
   */
  inline unsigned char* data() const { return myData; }

  /**
   * @brief Replace image RGBA data
   * @param width The new image width
   * @param height The new image height
   *
   * This call will replace the old data with the new data. Data should 
   * match the size width * height * 4
   */
  void set(int width, int height, unsigned char* data);

  /**
   * @brief Get the pixel at index (row, col)
   * @param row The row (value between 0 and height)
   * @param col The col (value between 0 and width)
   *
   * Pixel colors are unsigned char, e.g. in range 0 to 255
   */ 
  Pixel get(int row, int col) const;

  /**
   * @brief Set the pixel RGBA color at index (row, col)
   * @param row The row (value between 0 and height)
   * @param col The col (value between 0 and width)
   *
   * Pixel colors are unsigned char, e.g. in range 0 to 255
   * @verbinclude checker.cpp
   */ 
  void set(int row, int col, const Pixel& color);

  /**
   * @brief Set the pixel RGBA color at index (row, col)
   * @param row The row (value between 0 and height)
   * @param col The col (value between 0 and width)
   *
   * vec4 colors are in range [0,1] 
   */ 
  void setVec4(int row, int col, const glm::vec4& color);

  /**
   * @brief Get the pixel RGBA color at index (row, col)
   * @param row The row (value between 0 and height)
   * @param col The col (value between 0 and width)
   *
   * vec4 colors are in range [0,1] 
   */ 
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
