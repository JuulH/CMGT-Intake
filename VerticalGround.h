#pragma once
#include "template.h"
#include "surface.h"

namespace Tmpl8{
class VerticalGround
{
public:
	VerticalGround();
	VerticalGround(vec2 start, vec2 end, Pixel color, bool left = false, int id = 0);
	void Draw(Tmpl8::Surface* screen);

	vec2 start;
	vec2 end;
	float length;
	Pixel color;
	int id = 0;
	bool left;
};

}