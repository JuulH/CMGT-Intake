#include "VerticalGround.h"

namespace Tmpl8 {

	VerticalGround::VerticalGround() : start(vec2(0, 0)), end(vec2(0, 0)), color(0x000000) {
		length = 0;
		id = 0;
		left = false;
	}

	VerticalGround::VerticalGround(vec2 _start, vec2 _end, Pixel _color, bool _left, int _id) {
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
}
