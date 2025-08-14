#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Renderer {
private:
  SDL_Renderer *renderer;

public:
  Renderer();
  ~Renderer();

  bool Initialize(SDL_Window *window);
  void Cleanup();
  void Clear(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 = 255);
  void Present();
  void DrawTexture(SDL_Texture *texture, const SDL_Rect *src,
                   const SDL_Rect *dst);
  void DrawRect(const SDL_Rect &rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

  SDL_Renderer *GetSDLRenderer() const { return renderer; }
};
