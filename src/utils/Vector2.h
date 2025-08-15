#pragma once

struct Vector2 {
  float x, y;

  Vector2(float x = 0, float y = 0) : x(x), y(y) {}

  Vector2 operator+(const Vector2 &other) const;
  Vector2 operator-(const Vector2 &other) const;
  Vector2 operator*(const Vector2 &other) const;

  float Length() const;
  Vector2 Normalized() const;
};
