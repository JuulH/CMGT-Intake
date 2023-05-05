#include "Ground.h"

namespace Tmpl8 {

	Ground::Ground() : start(vec2(0, 0)), end(vec2(0, 0)), color(0x000000) {
		length = 0;
		angle = 0;
		middle = vec2(0, 0);
	}

	Ground::Ground(vec2 _start, vec2 _end, Pixel _color) {
		start = _start;
		end = _end;
		color = _color;

		length = sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
		angle = atan2(end.y - start.y, end.x - start.x);
		middle.x = (start.x + end.x) / 2;
		middle.y = (start.y + end.y) / 2;
	}

	void Ground::Draw(Tmpl8::Surface* screen) {
		screen->Line(start.x, start.y, end.x, end.y, color);
		screen->Box(middle.x - 5, middle.y - 5, middle.x + 5, middle.y + 5, 0xff0000);
	}

}