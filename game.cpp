#include "game.h"
#include "surface.h"
#include "template.h"
#include "input.h"
#include "ball.h"
#include "ground.h"
#include <cstdio> //printf
#include <time.h>

namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------

	constexpr int segments = 10;
	Ground groundSegments[segments];

	void Game::Init()
	{
		float segmentWidth = 1280 / segments;
		float startY = 720 - 150;

		srand(time(NULL));
		for (int i = 0; i < segments; i++) {
			float endY = startY + rand() % 200 - 100;
			groundSegments[i] = Ground(vec2(segmentWidth * i, startY), vec2(segmentWidth * (i + 1), endY), 0x000000);
			startY = endY;
		}
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
	
	Ball b;

	void Game::Tick(float deltaTime)
	{
		screen->Clear(background);

		b.HandleInput(screen);
		b.Update(deltaTime);
		b.ScreenCollisions();

		for (Ground g : groundSegments) {
			g.Draw(screen);
			b.GroundCollisions(g);
		}

		b.Draw(screen, &ball);

		//vx *= pow(0.9994f, deltaTime);
		//vy *= pow(0.997f, deltaTime);

		/*if (abs(vx) < 0.005f) vx = 0;
		if (abs(vy) < 0.005f) vy = 0;*/

		Input::Update();
	}
};