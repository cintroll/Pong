#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <memory>
#include <vector>

class Pong {
private:
  bool isRunning;

  // std::vector<std::unique_ptr<void>> gameObjects;

public:
  Pong();
  ~Pong();

  bool Initialize();
  void HandleEvents();
  void Update(float deltaTime);
  void Render();
  void Cleanup();

  bool IsRunning() const { return isRunning; }
};
