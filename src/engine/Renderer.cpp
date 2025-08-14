#include "Renderer.h"
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#include <iostream>

Renderer::Renderer() : renderer(nullptr) {}

Renderer::~Renderer() { Cleanup(); }

bool Renderer::Initialize(SDL_Window *window) {
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!renderer) {
    std::cerr << "Erro ao criar renderer: " << SDL_GetError() << std::endl;
    return false;
  }

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  return true;
}

void Renderer::Cleanup() {
  if (renderer) {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
  }
}

void Renderer::Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderClear(renderer);
}

void Renderer::Present() { SDL_RenderPresent(renderer); }

void Renderer::DrawTexture(SDL_Texture *texture, const SDL_Rect *src,
                           const SDL_Rect *dest) {
  SDL_RenderCopy(renderer, texture, src, dest);
}

void Renderer::DrawRect(const SDL_Rect &rect, Uint8 r, Uint8 g, Uint8 b,
                        Uint8 a) {
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderFillRect(renderer, &rect);
}
