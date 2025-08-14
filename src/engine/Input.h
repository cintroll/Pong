#pragma once
#include <SDL2/SDL.h>

class Input {
private:
  const Uint8 *keyboardState;
  Uint8 *previouskeyboardState;
  int numKeys;
  Uint32 mouseState;
  Uint32 previousMouseState;
  int mouseX, mouseY;

public:
  Input();
  ~Input();

  void Initialize();
  void Update();
  void Cleanup();

  bool IsKeyPressed(SDL_Scancode key) const;
  bool IsKeyJustPressed(SDL_Scancode key) const;
  bool IKeyReleased(SDL_Scancode key) const;

  bool isMouseButtonPressed(int button) const;
  bool IsMouseButtonJustPressed(int button) const;
  void GetMousePosition(int &x, int &y) const;
};
