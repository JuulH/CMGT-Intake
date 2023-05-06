#include "Ball.h"
#include "input.h"
#include "surface.h"
#include <Windows.h>

namespace Tmpl8{

void Ball::HandleInput(Tmpl8::Surface* screen) {
	mousePosition = Input::GetMousePosition();
	bool ballMoving = v.length() > 0.03f;

	if (!ballMoving) {
		if (Input::GetMouseButtonDown(SDL_BUTTON_LEFT)) {
			mouseStart = mousePosition;
			startedAiming = true;
		}

		else if (Input::GetMouseButton(SDL_BUTTON_LEFT) && startedAiming) {
			mouseEnd = mousePosition;
			direction = mouseEnd - mouseStart;
			if (direction.length() > 300) direction = direction.normalized() * 300;
			screen->Line(pos.x, pos.y, pos.x - direction.x, pos.y - direction.y, 0xffffff);
		}

		else if (Input::GetMouseButtonUp(SDL_BUTTON_LEFT) && startedAiming) {
			direction *= -0.009f;
			v.x = direction.x;
			v.y = direction.y;
			startedAiming = false;
		}
	}
}

void Ball::Update(float deltaTime) {
	// Friction
	a.x = -v.x * friction;
	a.y = -v.y * friction + gravity;

	// Stop ball if velocity is too low
	//if (abs(v.x) < 0.008f) v.x = 0;
	//if (a.y == .0035f && abs(v.y) < 0.025f) v.y = 0;

	// Velocity
	v.x += a.x * deltaTime;
	v.y += a.y * deltaTime;

	//if (v.length() < 0.007f) v.x = 0, v.y = 0;

	// Position
	pos.x += v.x * deltaTime;
	pos.y += v.y * deltaTime;

	/*previous += pos;
	if (ignoreBounce) {
		if ((previous - pos).length() > 0.3f) ignoreBounce = false;
	}*/
}

void Ball::ScreenCollisions() {
	if (pos.x < radius) {
		pos.x = radius;
		v.x = -v.x * bounciness;
	}
	if (pos.x > ScreenWidth - radius) {
		pos.x = ScreenWidth - radius;
		v.x = -v.x * bounciness;
	}
	if (pos.y < radius) {
		pos.y = radius;
		v.y = -v.y * bounciness;
	}
	if (pos.y > ScreenHeight - radius) {
		pos.y = ScreenHeight - radius;
		v.y = -v.y * bounciness;
	}
}

void Ball::GroundCollisions(Ground g) {
	if(pos.x - 1 > g.end.x || pos.x + 1 < g.start.x) return;

	float dx = pos.x - g.middle.x;
	float dy = pos.y - g.middle.y;

	float cosine = cos(g.angle);
	float sine = sin(g.angle);

	float dx_ = cosine * dx + sine * dy;
	float dy_ = cosine * dy - sine * dx;
	float velx_ = cosine * v.x + sine * v.y;
	float vely_ = -sine * v.x + cosine * v.y;

	if (dy_ > -radius && pos.x + radius >= g.start.x && pos.x - radius <= g.end.x) {
		dy_ = -radius;
		if (abs(vely_) > 0.02f/* && !ignoreBounce*/) vely_ *= -bounciness;
		else vely_ = 0;
	}

	dx = cosine * dx_ - sine * dy_;
	dy = cosine * dy_ + sine * dx_;

	v.x = cosine * velx_ - sine * vely_;
	v.y = cosine * vely_ + sine * velx_;

	pos.x = g.middle.x + dx;
	pos.y = g.middle.y + dy;

	/*ignoreBounce = true;
	previous = vec2(g.middle.x + dx, g.middle.y + dy);*/
}

void Ball::Draw(Tmpl8::Surface* screen, Tmpl8::Sprite* sprite) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 2 });
	printf("Ball:\nx: %f, y: %f\nvx: %f, vy: %f\nax: %f, ay: %f\np: %f\n", pos.x, pos.y, v.x, v.y, a.x, a.y, abs((pos - previous).length()));
	sprite->DrawScaled(pos.x - radius, pos.y - radius, 26, 26, screen);
}

}