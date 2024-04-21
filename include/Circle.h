#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Vec2.h"


class Particle : public sf::CircleShape {
protected:
  Particle() {
    init();
  }

  bool operator!=(Particle p) {
    return id_ != p.id_;
  }

  void init(); // Initialize particles
  void update(float dt, std::vector<Particle>& particles);

  bool collision(const Particle& other);

  bool borderCollision();

  sf::Vector2f velocity_;
  float mass_;
  bool collided_;
  sf::Color colour_ = sf::Color::Green;
  unsigned int id_;

  friend class ParticleSystem;
};