#pragma once
#include "template.h"
#include "surface.h"

class UIElement
{

public:
	virtual bool HandleInput(Tmpl8::vec2 mousePosition) { return false; };
	virtual void Draw(Tmpl8::Surface* screen, Tmpl8::vec2 position) {};

	bool isDirty = false; // Dirty flag to update buffer
	int anchor = 0;
	Tmpl8::vec2 relPos = Tmpl8::vec2(0, 0);
	float width = 0;
	float height = 0;
	Tmpl8::vec2 pos = Tmpl8::vec2(0, 0);
	bool active = true;
};