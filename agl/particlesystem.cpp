#include "particlesystem.h"
#include "AGLM.h"

using namespace glm;
using namespace std;
using namespace agl;

ParticleSystem::ParticleSystem()
{
   mBlendMode = ADD;
   mTexture = -1;
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::init(int size)
{
   createParticles(size);
}

void ParticleSystem::draw() 
{
   Renderer::Get().begin(mTexture, mBlendMode);
   for (int i = 0; i < mParticles.size(); i++)
   {
      Particle particle = mParticles[i];
      Renderer::Get().quad(particle.pos, particle.color, particle.size);
   }
}
