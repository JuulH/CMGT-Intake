#include "game.h"
#include "surface.h"
#include "template.h"
#include "input.h"
#include <cstdio> //printf

namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------

	void Game::Init()
	{
	}
	
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	Sprite ball(new Surface("assets/golfball.png"), 1);
	Pixel background = 0x006f00;
	vec2 mouseStart;
	vec2 mouseEnd;
	vec2 direction;

	float ay = 0.0035f;

	float vx = 0;
	float vy = 0;

	float x = 1280/2;
	float y = 720/2;

	float bounciness = 0.5f;

	void Game::Tick(float deltaTime)
	{
		screen->Clear(background);

		vec2 mousePosition = Input::GetMousePosition();

		bool ballMoving = abs(vx) > 0.01f || abs(vy) > 0.01f;

		if (!ballMoving) {
			if (Input::GetMouseButtonDown(SDL_BUTTON_LEFT)) {
				mouseStart = mousePosition;
			}

			else if (Input::GetMouseButton(SDL_BUTTON_LEFT)) {
				mouseEnd = mousePosition;
				direction = mouseEnd - mouseStart;
				if (direction.length() > 300) direction = direction.normalized() * 300;
				screen->Line(x, y, x - direction.x, y - direction.y, 0xffffff);
			}

			else if (Input::GetMouseButtonUp(SDL_BUTTON_LEFT)) {
				direction *= -0.009f;
				vx = direction.x;
				vy = direction.y;
			}
		}

		vx *= pow(0.9992f, deltaTime);
		//vy *= pow(0.997f, deltaTime);

		vy += ay * deltaTime;

		if (abs(vx) < 0.005f) vx = 0;
		if (abs(vy) < 0.005f) vy = 0;

		x += vx * deltaTime;
		y += vy * deltaTime;

		if (x - 13 < 0) {
			x = 13;
			if(abs(vx * -1 * bounciness) < 0.1f) vx = 0;
			else vy *= bounciness, vx *= -1 * bounciness;
		} else if (x + 13 > 1280) {
			x = 1280 - 13;
			if (abs(vx * -1 * bounciness) < 0.1f) vx = 0;
			else vy *= bounciness, vx *= -1 * bounciness;
		}

		if (y - 13 < 0) {
			y = 13;
			if (abs(vy * -1 * bounciness) < 0.1f) vy = 0;
			else vy *= -1 * bounciness, vx *= bounciness;
		} else if (y + 13 > 720) {
			y = 720 - 13;
			if (abs(vy * -1 * bounciness) < 0.1f) vy = 0;
			else vy *= -1 * bounciness, vx *= bounciness;
		}

		ball.DrawScaled((int) x - 13, (int) y - 13, 26, 26, screen);

		Input::Update();
	}
};