#ifndef Scene_H_
#define Scene_H_

#include "AGL.h"
#include "image.h"
#include "renderer.h"
#include <vector>

namespace agl {

   class Scene {
   public:
      Scene();
      virtual ~Scene();

      void init();
      virtual void update(float dt);
      virtual void draw();

      static Renderer& GetRenderer();
      
   protected:

      // todo: your stuff here
      ParticleSystem mSystem;
      Mesh mMesh;
      static Renderer theRenderer;
   };
}
#endif
