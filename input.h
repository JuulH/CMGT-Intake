#pragma once
#include "template.h"
#include <SDL_keycode.h>
#include <SDL_mouse.h>

constexpr int keys = SDL_NUM_SCANCODES;
constexpr int mouseButtons = 5;

class Input
{
public:
	static bool GetKeyDown(SDL_Scancode key);
	static bool GetKeyUp(SDL_Scancode key);
	static bool GetKey(SDL_Scancode key);
	static void SetKeyState(SDL_Scancode key, bool state);

	static bool GetMouseButtonDown(int button);
	static bool GetMouseButtonUp(int button);
	static bool GetMouseButton(int button);
	static void SetMouseState(int button, bool state);

	static void SetMouseWheel(int y) { mouseWheel = y; }
	static int GetMouseWheel() { return mouseWheel; }

	static Tmpl8::vec2 GetMousePosition() { return mousePosition; }
	static void SetMousePosition(Sint32 x, Sint32 y) { mousePosition.x = static_cast<float>(x), mousePosition.y = static_cast<float>(y); }

	static void Update();

private:
	static int keyStates[];
	static int mouseStates[];
	static Tmpl8::vec2 mousePosition;
	static int mouseWheel;
};