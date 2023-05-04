#pragma once
#include "template.h"
#include <SDL_keycode.h>
#include <SDL_mouse.h>

constexpr int keys = 261;
constexpr int mouseButtons = 5;

namespace Tmpl8 {

class Input
{
public:
	static bool GetKeyDown(int key);
	static bool GetKeyUp(int key);
	static bool GetKey(int key);
	static void SetKeyState(int key, bool state);

	static bool GetMouseButtonDown(int button);
	static bool GetMouseButtonUp(int button);
	static bool GetMouseButton(int button);
	static void SetMouseState(int button, bool state);

	static void SetMouseWheel(int y) { mouseWheel = y; }
	static int GetMouseWheel() { return mouseWheel; }

	static vec2 GetMousePosition() { return mousePosition; }
	static void SetMousePosition(int x, int y) { mousePosition.x = x, mousePosition.y = y; }

	static void Update();

private:
	static int keyStates[];
	static int mouseStates[];
	static vec2 mousePosition;
	static int mouseWheel;
};

}
