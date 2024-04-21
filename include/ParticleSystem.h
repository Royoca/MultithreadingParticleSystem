#pragma once
#include <vector>
#include "Constants.h"
#include "Circle.h"

class ParticleSystem {
public:
  
  ParticleSystem(const unsigned int size) { // Create the particle system with a desired number of particles
    for (unsigned int i = 0; i < kn_particles; i++) {
      particle_pool_.push_back(Particle());
    }
  }

  // Wrapper static function for updating the particles, used for multithreading
  static void UpdateParticles(ParticleSystem& particles, float dt, unsigned int start, unsigned int end) {
    particles.updateRange(dt, start, end);
  }

  void update(float dt); // Update all particles
  void updateRange(float dt, unsigned int start, unsigned int end); // Update only a range of particles
  void draw(sf::RenderWindow& window); // Draw all particles
  void drawRange(sf::RenderWindow& window, unsigned int start, unsigned int end); // Draw only a range of particles

private:

  std::vector<Particle> particle_pool_;

};
