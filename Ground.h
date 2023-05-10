#pragma once
#include "template.h"
#include "surface.h"

class Ground
{
public:
	Ground();
	Ground(Tmpl8::vec2 start, Tmpl8::vec2 end, Tmpl8::Pixel color, int id = 0);
	void ComputeProperties();
	void Draw(Tmpl8::Surface* screen);

	float length;
	float angle;
	Tmpl8::vec2 middle;
	Tmpl8::vec2 start;
	Tmpl8::vec2 end;
	Tmpl8::Pixel color;
	int id = 0;
};