#include "Pong.h"
#include <SDL2/SDL_image.h>
#include <iostream>

Pong::Pong() : isRunning{false} {}

Pong::~Pong() { Cleanup(); }

bool Pong::Initialize() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
    return false;
  }

  int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
  if (IMG_Init(imgFlags) != 0) {
    std::cerr << "Erro ao inicializar SDL_image: " << IMG_GetError()
              << std::endl;
  }

  isRunning = true;

  std::cout << "Jogo inicializado com sucesso!" << std::endl;
  return true;
}

void Pong::Cleanup() {}
