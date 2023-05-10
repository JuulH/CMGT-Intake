#include "Button.h"

Button::Button(UIContainer::Anchor _anchor, Tmpl8::vec2 _relPos, Tmpl8::Sprite* _sprite, float _scale, std::function<void()> _func) {
	anchor = _anchor;
	relPos = _relPos;
	sprite = _sprite;
	scale = _scale;
	width = sprite->GetWidth() * scale;
	height = sprite->GetHeight() * scale;
	func = _func;
	status = 0;
}

bool Button::HandleInput(Tmpl8::vec2 mousePos) {
	// If mouse is within bounds
	if (mousePos.x > pos.x && mousePos.x < pos.x + width &&
		mousePos.y > pos.y && mousePos.y < pos.y + height) {
		
		// Handle input
		if (status != HOVERING && status != FOCUSED && !Input::GetMouseButtonDown(SDL_BUTTON_LEFT)) {
			SetStatus(HOVERING);
		} else if (Input::GetMouseButtonDown(SDL_BUTTON_LEFT)) {
			SetStatus(FOCUSED);
		} else if (Input::GetMouseButtonUp(SDL_BUTTON_LEFT)) {
			if (status == FOCUSED) func();
			SetStatus(HOVERING);
		}

		return true; // Prevent game input
	}
	else {
		SetStatus(INACTIVE);
		return false;
	}
}

void Button::Draw(Tmpl8::Surface* screen, Tmpl8::vec2 position) {
	sprite->SetFrame(status);
	sprite->DrawScaled(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(width), static_cast<int>(height), screen);
}

void Button::SetStatus(int _status) {
	if (status != _status) {
		status = _status;
		isDirty = true;
	}
}