#pragma once
#include "template.h"
#include "surface.h"

namespace Tmpl8 {

class Ground
{
public:
	Ground();
	Ground(vec2 start, vec2 end, Pixel color, int id = 0);
	void ComputeProperties();
	void Draw(Tmpl8::Surface* screen);

	float length;
	float angle;
	vec2 middle;
	vec2 start;
	vec2 end;
	Pixel color;
	int id = 0;
};

}