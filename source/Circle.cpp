#include "Circle.h"
#include "Vec2.h"
#include "Constants.h"
#include <random>
#include <chrono>   // for 'time'.

std::mt19937 random_number_engine(time(0));

float rng(float min, float max)
{
  std::uniform_real_distribution<float> distribution(min, max);
  return distribution(random_number_engine);
}

float rng0to255()
{
  std::uniform_int_distribution<unsigned int> distribution(0, 255);
  return distribution(random_number_engine);
}

void Particle::init() {
  setOrigin(sf::Vector2(getRadius(), getRadius()));
  float dir_x = rng(-1.0f, 1.0f);
  float dir_y = rng(-1.0f, 1.0f);
  float pos_x = rng(0.0f, k_screen_x);
  float pos_y = rng(0.0f, k_screen_y);
  velocity_ = sf::Vector2(dir_x, dir_y);
  velocity_ *= rng(k_min_speed, k_max_speed);
  setRadius(rng(k_min_radius, k_max_radius));
  mass_ = getRadius();
  setPosition(sf::Vector2(pos_x, pos_y));
  setFillColor(sf::Color(rng0to255(), rng0to255(), rng0to255()));
  static unsigned int id = 0;
  id_ = id;
  id++;
}

void Particle::update(float dt, std::vector<Particle>& particles) {
  collided_ = false;

  // Move the particle
  move(velocity_ * dt);

  // Iterate over the other particles and affect the collided ones with elastic collision behaviour
  for (Particle& p : particles) {
    if (collision(p) && p != *this && collided_ == false) {
      collided_ = true;
      sf::Vector2f normal = p.getPosition() - getPosition();
      float magnitude = std::sqrt(normal.x * normal.x + normal.y * normal.y);
      normal /= magnitude;
      sf::Vector2f tangent(-normal.y, normal.x);

      // Project the velocities onto the normal and tangent vectors
      float dot_tan1 = velocity_.x * tangent.x + velocity_.y * tangent.y;
      float dot_normal1 = velocity_.x * normal.x + velocity_.y * normal.y;
      float dot_tan2 = p.velocity_.x * tangent.x + p.velocity_.y * tangent.y;
      float dot_normal2 = p.velocity_.x * normal.x + p.velocity_.y * normal.y;

      // Calculate new normal velocities using the conservation of momentum
      float m1 = (dot_normal1 * (mass_ - p.mass_) + 2 * p.mass_ * dot_normal2) / (mass_ + p.mass_);
      float m2 = (dot_normal2 * (p.mass_ - mass_) + 2 * mass_ * dot_normal1) / (mass_ + p.mass_);

      // Convert scalar normal and tangent velocities into vectors
      sf::Vector2f newNormalVelocity1 = m1 * normal;
      sf::Vector2f newTangentVelocity1 = dot_tan1 * tangent;
      sf::Vector2f newNormalVelocity2 = m2 * normal;
      sf::Vector2f newTangentVelocity2 = dot_tan2 * tangent;

      // Update the velocities
      velocity_ = newNormalVelocity1 + newTangentVelocity1;
      p.velocity_ = newNormalVelocity2 + newTangentVelocity2;

    }
  }
}

bool Particle::collision(const Particle& other)
{
  sf::Vector2f pos1 = getPosition();
  sf::Vector2f pos2 = other.getPosition();
  float dist = std::sqrt(std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2));
  return dist <= (getRadius() + other.getRadius());
}

bool Particle::borderCollision() {
  return getPosition().x > k_screen_x + getRadius()
    || getPosition().x < - getRadius()
    || getPosition().y > k_screen_y + getRadius()
    || getPosition().y < -getRadius();
}