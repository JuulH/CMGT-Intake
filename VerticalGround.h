#pragma once
#include "template.h"
#include "surface.h"

class VerticalGround
{
public:
	VerticalGround();
	VerticalGround(Tmpl8::vec2 start, Tmpl8::vec2 end, Tmpl8::Pixel color, bool left = false, int id = 0);
	void Draw(Tmpl8::Surface* screen);

	Tmpl8::vec2 start;
	Tmpl8::vec2 end;
	float length;
	Tmpl8::Pixel color;
	int id = 0;
	bool left;
};