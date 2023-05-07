#include "Button.h"

namespace Tmpl8 {
	Button::Button(vec2 _anchor, vec2 _relPos, Sprite* _sprite, float _scale, std::function<void()> _func) {
		anchor = _anchor;
		relPos = _relPos;
		sprite = _sprite;
		scale = _scale;
		width = sprite->GetWidth() * scale;
		height = sprite->GetHeight() * scale;
		func = _func;

		pos = vec2(anchor.x + relPos.x, anchor.y + relPos.y);
		status = 0;
	}

	bool Button::HandleInput(vec2 mousePos) {
		// Check if mouse is within button bounds
		if (mousePos.x > pos.x && mousePos.x < pos.x + width &&
			mousePos.y > pos.y && mousePos.y < pos.y + height) {

			status = 1; // Hovering
			
			// On click
			if (Input::GetMouseButtonDown(SDL_BUTTON_LEFT)) {
				func();
			} else if (Input::GetMouseButtonUp(SDL_BUTTON_LEFT)) {
				status = 0; // Released
			}

			if (Input::GetMouseButton(SDL_BUTTON_LEFT)) {
				status = 2; // Clicked
			}
			return true;
		}
		else {
			status = 0; // Not hovering
			return false;
		}
	}

	void Button::Draw(Surface* screen) {
		sprite->SetFrame(status);
		sprite->DrawScaled(pos.x, pos.y, width, height, screen);
		char buffer[50];
		sprintf(buffer, "%i", status);
		screen->Print(buffer, pos.x - 50, pos.y - 50, 0x000000, 3);
	}
}