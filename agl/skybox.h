// Copyright 2011, OpenGL 4.0 Shading language cookbook (David Wolf)

#ifndef AGL_SKYBOX_H_
#define AGL_SKYBOX_H_

namespace agl {

class SkyBox {
 public:
  explicit SkyBox(int size);
  void render() const;

 private:
  unsigned int vaoHandle;
};

}  // namespace agl
#endif  // AGL_SKYBOX_H_
