#include "Ground.h"

Ground::Ground() : start(Tmpl8::vec2(0, 0)), end(Tmpl8::vec2(0, 0)), color(0x000000) {
	length = 0;
	angle = 0;
	middle = Tmpl8::vec2(0, 0);
	id = 0;
}

Ground::Ground(Tmpl8::vec2 _start, Tmpl8::vec2 _end, Tmpl8::Pixel _color, int _id) {
	start = _start;
	end = _end;
	color = _color;
	id = _id;

	Ground::ComputeProperties();
}

/// <summary>
/// Calculate the length, angle and middle of the ground.
/// </summary>
void Ground::ComputeProperties() {
	length = sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
	angle = atan2(end.y - start.y, end.x - start.x);
	middle.x = (start.x + end.x) / 2;
	middle.y = (start.y + end.y) / 2;
}

void Ground::Draw(Tmpl8::Surface* screen) {
	screen->LineFill(start.x, start.y, end.x, end.y, color);
}