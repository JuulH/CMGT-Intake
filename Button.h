#pragma once
#include "template.h"
#include "surface.h"
#include "input.h"
#include "UIElement.h"
#include "UIContainer.h"
#include <functional>

class Button : public UIElement
{
public:
	Button(UIContainer::Anchor anchor, Tmpl8::vec2 relPos, Tmpl8::Sprite* sprite, float scale, std::function<void()> func);
	virtual bool HandleInput(Tmpl8::vec2 mousePos) override;
	virtual void Draw(Tmpl8::Surface* screen, Tmpl8::vec2 position) override;

	void SetStatus(int status);
	void SetFunc(std::function<void()> func) { this->func = func; }

private:
	enum ButtonState {
		INACTIVE,
		HOVERING,
		FOCUSED
	};

	Tmpl8::Sprite* sprite;
	float scale;
	std::function<void()> func;
	int status;
};