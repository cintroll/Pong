#include "Window.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_video.h>
#include <iostream>

Window::Window() : window(nullptr), width(0), height(0) {}

Window::~Window() { Destroy(); }

bool Window::Create(const std::string &title, int w, int h) {
  width = w;
  height = h;

  window =
      SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);

  if (!window) {
    std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
    return false;
  }

  return true;
}

void Window::Destroy() {
  if (window) {
    SDL_DestroyWindow(window);
    window = nullptr;
  }
}
