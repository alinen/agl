#ifndef Trail_H_
#define Trail_H_

#include "particlesystem.h"

namespace g3d {

   class Trail : public ParticleSystem {
   public:
      virtual void createParticles(int size) override;
      virtual void update(float dt) override;
   };
}
#endif
