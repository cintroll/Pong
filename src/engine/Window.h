#pragma once

#include <SDL2/SDL.h>
#include <string>

class Window {
private:
  SDL_Window *window;
  int width, height;

public:
  Window();
  ~Window();

  bool Create(const std::string &title, int w, int h);
  void Destroy();

  SDL_Window *GetSDLWindow() const { return window; }
  int GetWidth() const { return width; }
  int Getheight() const { return height; }
};
