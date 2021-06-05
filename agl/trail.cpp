// Bryn Mawr College, alinen, 2020
//

#include "AGL.h"
#include "AGLM.h"
#include <cmath>
#include "trail.h"
#include "renderer.h"

using namespace std;
using namespace agl;
using namespace glm;

void Trail::createParticles(int size) 
{
   mTexture = Renderer::Get().loadTexture("../textures/particle.png");

   Particle p;
   p.pos = vec3(0.0);
   p.color = vec4(0,0,1,1);
   p.size = 0.5;
   mParticles.push_back(p);
}

void Trail::update(float dt)
{
  // update particles
}

