#pragma once
#include "template.h"
#include "surface.h"
#include "input.h"
#include <functional>

namespace Tmpl8 {

class Button
{
public:
	Button(vec2 anchor, vec2 relPos, Sprite* sprite, float scale, std::function<void()> func);
	bool HandleInput(vec2 mousePos);
	void Draw(Surface* screen);
private:
	enum Anchor {
		TOPLEFT = (0, 0),
		TOPRIGHT = (ScreenWidth, 0),
		BOTTOMLEFT = (0, ScreenHeight),
		BOTTOMRIGHT = (ScreenWidth, ScreenHeight),
		CENTER = (ScreenWidth / 2, ScreenHeight / 2)
	};

	vec2 anchor;
	vec2 relPos;
	float width;
	float height;
	Sprite* sprite;
	float scale;
	std::function<void()> func;

	vec2 pos;
	int status;
};

}