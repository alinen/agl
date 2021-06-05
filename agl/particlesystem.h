#ifndef ParticleSystem_H_
#define ParticleSystem_H_

#include "AGL.h"
#include "image.h"
#include "renderer.h"
#include <vector>

namespace agl {

   struct Particle {
      glm::vec3 pos;
      glm::vec4 color;
      float size;
      // todo: your stuff here
   };

   class ParticleSystem {
   public:
      ParticleSystem();
      virtual ~ParticleSystem();

      void init(int size);
      virtual void update(float dt) = 0;
      virtual void draw();

   protected:
      virtual void createParticles(int size) = 0;

   protected:

      std::vector<Particle> mParticles;
      GLuint mTexture;
      BlendMode mBlendMode;
   };
}
#endif
