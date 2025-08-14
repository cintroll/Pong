#include "Input.h"
#include <SDL2/SDL_mouse.h>
#include <cstring>

Input::Input()
    : keyboardState(nullptr), previouskeyboardState(nullptr), numKeys(0),
      mouseState(0), previousMouseState(0), mouseX(0), mouseY(0) {}

Input::~Input() { Cleanup(); }

void Input::Initialize() {
  keyboardState = SDL_GetKeyboardState(&numKeys);
  previouskeyboardState = new Uint8[numKeys];
  std::memset(previouskeyboardState, 0, numKeys);
}

void Input::Update() {
  std::memcpy(previouskeyboardState, keyboardState, numKeys);

  previousMouseState = mouseState;

  keyboardState = SDL_GetKeyboardState(nullptr);
  mouseState = SDL_GetMouseState(&mouseX, &mouseY);
}
void Input::Cleanup() {
  if (previousMouseState) {
    delete[] previouskeyboardState;
    previouskeyboardState = nullptr;
  }
}

bool Input::IsKeyPressed(SDL_Scancode key) const { return keyboardState[key]; }

bool Input::IsKeyJustPressed(SDL_Scancode key) const {
  return keyboardState[key] && !previouskeyboardState[key];
}

bool Input::IKeyReleased(SDL_Scancode key) const {
  return !keyboardState[key] && previouskeyboardState[key];
}

bool Input::isMouseButtonPressed(int button) const {
  return mouseState & SDL_BUTTON(button);
}

bool Input::IsMouseButtonJustPressed(int button) const {
  return (mouseState & SDL_BUTTON(button)) &&
         !(previousMouseState & SDL_BUTTON(button));
}

void Input::GetMousePosition(int &x, int &y) const {
  x = mouseX;
  y = mouseY;
}
