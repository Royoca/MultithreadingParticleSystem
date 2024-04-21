#pragma once
#include <cmath>
#include <limits>

constexpr float EPSILON = std::numeric_limits<float>::epsilon();

class Vec2 {
public:

  Vec2() { x_ = 0.0f, y_ = 0.0f; }
  Vec2(float x, float y) { x_ = x, y_ = y; }

  Vec2 operator-(Vec2 v) {
    return Vec2(x_ - v.x_, y_ - v.y_);
  }

  Vec2 operator*(float f) const{
    return Vec2(x_ * f, y_ * f);
  }

  Vec2& operator*=(float f) {
    x_ *= f;
    y_ *= f;
    return *this;
  }

  float sqrMagnitude() const;
  float magnitude() const;

  void normalize();
  Vec2 normalized() const;

  float x_, y_;
};

inline float Vec2::sqrMagnitude() const {
  return (float)((x_ * x_) + (y_ * y_));
}

inline float Vec2::magnitude() const {
  return (float)sqrt(sqrMagnitude());
}

inline void Vec2::normalize() {
  float inverse_mag = 1.0f / magnitude();
  x_ *= inverse_mag;
  y_ *= inverse_mag;
}

inline Vec2 Vec2::normalized() const {
  float sqr_mag = sqrMagnitude();
  if (sqr_mag >= 1.0f - EPSILON && sqr_mag <= 1.0f + EPSILON) return *this;
  float inverse_mag = 1.0f / magnitude();
  Vec2 r(x_ * inverse_mag, y_ * inverse_mag);
  return r;
}