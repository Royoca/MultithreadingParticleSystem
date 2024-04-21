#include "ParticleSystem.h"
#include <SFML/Graphics.hpp>
#include <mutex>
#include <algorithm>
#include <windows.h>

void ParticleSystem::update(float dt) {
  for (Particle& p : particle_pool_) {
    p.update(dt, particle_pool_);
  }
}

void ParticleSystem::updateRange(float dt, unsigned int start, unsigned int end) {

  static std::mutex cs;

  /////////////////////////////////////////////////
  // Lock with mutex, same as CRITICAL_SECTION
  /////////////////////////////////////////////////
  cs.lock();

  for (unsigned int i = start; i < end; i++) {
    if (particle_pool_[i].borderCollision()) {

      // Replace particles that are outside the displaying area
      particle_pool_.erase(particle_pool_.begin() + i);
      particle_pool_.push_back(Particle());
      i--;
    }
  }

  cs.unlock();

  /////////////////////////////////////////////////
  // End of CRITICAL_SECTION
  /////////////////////////////////////////////////

  // Iterate through the particles in the range
  for (unsigned int i = start; i < end; i++) {
    particle_pool_[i].update(dt, particle_pool_);
  }
}

void ParticleSystem::draw(sf::RenderWindow& window) {
  for (Particle& p : particle_pool_) {
    window.draw(p);
  }
}

void ParticleSystem::drawRange(sf::RenderWindow& window, unsigned int start, unsigned int end) {
  for (unsigned int i = start; i < end; i++) {
    window.draw(particle_pool_[i]);
  }
}
