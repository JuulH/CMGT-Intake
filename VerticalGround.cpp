#include "VerticalGround.h"

VerticalGround::VerticalGround() : start(Tmpl8::vec2(0, 0)), end(Tmpl8::vec2(0, 0)), color(0x000000) {
	length = 0;
	id = 0;
	left = false;
}

VerticalGround::VerticalGround(Tmpl8::vec2 _start, Tmpl8::vec2 _end, Tmpl8::Pixel _color, bool _left, int _id) {
	start = _start;
	end = _end;
	color = _color;
	length = sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
	id = _id;
	left = _left;
}

void VerticalGround::Draw(Tmpl8::Surface* screen) {
	screen->LineFill(start.x, start.y, end.x, end.y, color);
}
