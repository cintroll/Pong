#pragma once
#include "../engine/Renderer.h"
#include "../utils/Vector2.h"

class GameObject {
protected:
  Vector2 position;
  Vector2 velocity;
  float rotation;
  bool active;

public:
  GameObject(float x = 0, float y = 0);
  virtual ~GameObject() = default;

  virtual void Update(float deltaTime) = 0;
  virtual void Render(Renderer *renderer) = 0;

  Vector2 GetPosition() const { return position; }
  void SetPosition(const Vector2 &pos) { position = pos; }
  bool IsActive() const { return active; }
};
