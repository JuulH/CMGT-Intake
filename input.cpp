#include "input.h"
	
int Input::keyStates[keys]{}; // https://wiki.libsdl.org/SDL2/SDLKeycodeLookup
int Input::mouseStates[mouseButtons]{}; // https://wiki.libsdl.org/SDL2/SDL_MouseButtonEvent
int Input::mouseWheel = 0;
Tmpl8::vec2 Input::mousePosition = Tmpl8::vec2(0, 0);

bool Input::GetKeyDown(SDL_Scancode key) {
	if (key < 0 || key > keys) return false;
	return keyStates[key] == 1;
}
	
bool Input::GetKeyUp(SDL_Scancode key) {
	if (key < 0 || key > keys) return false;
	return keyStates[key] == 3;
}
	
bool Input::GetKey(SDL_Scancode key) {
	if (key < 0 || key > keys) return false;
	return keyStates[key] == 2 || keyStates[key] == 1;
}

bool Input::GetMouseButtonDown(int button) {
	return mouseStates[button] == 1;
}

bool Input::GetMouseButtonUp(int button) {
	return mouseStates[button] == 3;
}

bool Input::GetMouseButton(int button) {
	return mouseStates[button] == 2 || mouseStates[button] == 1;
}

// 0 = inactive, 1 = pressed, 2 = held, 3 = released
void Input::SetKeyState(SDL_Scancode key, bool state) {
	if (key < 0 || key > keys) return;
	if (keyStates[key] == 2 && state) return; // Key already held
	keyStates[key] = state ? 1 : 3;
}

void Input::SetMouseState(int button, bool state) {
	if (button < 0 || button > mouseButtons) return;
	if (mouseStates[button] == 2 && state) return; // Button already held
	mouseStates[button] = state ? 1 : 3;
}

/// <summary>
/// Update states of keys and buttons for held and released states.
/// </summary>
void Input::Update() {
	for (int i = 0; i < keys; i++) {
		if (keyStates[i] == 1) keyStates[i] = 2; // Pressed -> Held
		else if (keyStates[i] == 3) keyStates[i] = 0; // Released -> Up
	}

	for (int i = 0; i < mouseButtons; i++) {
		if (mouseStates[i] == 1) mouseStates[i] = 2; // Pressed -> Held
		else if (mouseStates[i] == 3) mouseStates[i] = 0; // Released -> Up
	}

	mouseWheel = 0;
}