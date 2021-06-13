#include "agl/window.h"
#include <algorithm>

using namespace glm;
using namespace agl;

class MyWindow : public Window {
  void setup() {
    renderer.loadTexture("particle", "../textures/particle.png", 0);
    renderer.texture("image", "particle");
    renderer.blendMode(ADD);

    for (int i = 0; i < _numParticles; i++) {
      Particle particle;                                                        
      particle.size = 0.25 * agl::random() + 0.1;                                                     
      particle.pos = randomUnitCube();                                        
      particle.vel = randomUnitCube();                                        
      particle.color = vec4(randomUnitCube(), 1);                             
      _particles.push_back(particle);              
    }
  }

  void draw() {
    for (int i = 0; i < _particles.size(); i++) {                                                  
      Particle particle = _particles[i];       
      particle.pos += particle.vel * dt();                                        

      if (particle.pos.x < -1) particle.vel.x = abs(particle.vel.x);
      if (particle.pos.y < -1) particle.vel.y = abs(particle.vel.y);
      if (particle.pos.z < -1) particle.vel.z = abs(particle.vel.z);

      if (particle.pos.x > 1) particle.vel.x = -abs(particle.vel.x);
      if (particle.pos.y > 1) particle.vel.y = -abs(particle.vel.y);
      if (particle.pos.z > 1) particle.vel.z = -abs(particle.vel.z);
      _particles[i] = particle;                                                 
    }                                                                            
                                                                                
    vec3 cameraPos = renderer.cameraPosition();                               
    std::sort(_particles.begin(), _particles.end(),
       [cameraPos](Particle p1, Particle p2) {
         float dSqr1 = glm::length2(p1.pos - cameraPos);                         
         float dSqr2 = glm::length2(p2.pos - cameraPos);                         
         return (dSqr1 > dSqr2);
    });
    renderer.beginShader("sprite");
    for (int i = 0; i < _particles.size(); i++) {
      renderer.sprite(_particles[i].pos, 
          _particles[i].color, _particles[i].size);
    }
    renderer.endShader();
  }

 private:
  struct Particle {
    vec3 pos;
    vec3 vel;
    vec4 color;
    float size;
  };
  std::vector<Particle> _particles;
  int _numParticles = 500;
};

int main() {
  MyWindow window;
  window.run();
}
