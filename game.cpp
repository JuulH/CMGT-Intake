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

	constexpr int segments = 12;
	Ground groundSegments[segments + 3];

	int holeRangeStart = 8;
	int holeRangeEnd = 11;

	void Game::Init()
	{
		float segmentWidth = 1280 / segments;
		float startY = 720 - 150;

		srand(time(NULL));
		for (int i = 0; i < segments; i++) {
			float endY = startY + rand() % 200 - 100;
			if (endY > 720 - 150) endY -= 150;
			groundSegments[i] = Ground(vec2(segmentWidth * i, startY), vec2(segmentWidth * (i + 1), endY), i % 2 == 0 ? 0x005500 : 0x002200);
			startY = endY;
		}

		int holeSegment = rand() % (holeRangeEnd - holeRangeStart + 1) + holeRangeStart;

		groundSegments[holeSegment].color = 0xff0000;

		//// Get halfway point of hole segment
		//float holeX = (groundSegments[holeSegment].start.x + groundSegments[holeSegment].end.x) / 2;
		//float holeY = (groundSegments[holeSegment].start.y + groundSegments[holeSegment].end.y) / 2;

		//Ground hole[] = {
		//	Ground(vec2(holeX + -30, holeY), vec2(holeX + -30, holeY + 70), 0xffffff),
		//	Ground(vec2(holeX + -30, holeY + 70), vec2(holeX + 30, holeY + 70), 0xffffff),
		//	Ground(vec2(holeX + 30, holeY + 70), vec2(holeX + 30, holeY), 0xffffff),
		//};

		//groundSegments[holeSegment].end.x = hole[0].start.x;
		//groundSegments[holeSegment].end.y = hole[0].start.y;

		//groundSegments[holeSegment + 1].start.x = hole[2].end.x;
		//groundSegments[holeSegment + 1].start.y = hole[2].end.y;

		//// Add hole array to groundSegments array
		//for (int i = 0; i < 3; i++) {
		//	groundSegments[segments + i] = hole[i];
		//}

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
	Pixel background = 0x4466ff;
	
	Ball b;

	void Game::Tick(float deltaTime)
	{
		screen->Clear(background);

		b.HandleInput(screen);
		b.Update(deltaTime);

		for (Ground g : groundSegments) {
			g.Draw(screen);
			b.GroundCollisions(g);
		}
		b.ScreenCollisions();

		b.Draw(screen, &ball);

		//vx *= pow(0.9994f, deltaTime);
		//vy *= pow(0.997f, deltaTime);

		/*if (abs(vx) < 0.005f) vx = 0;
		if (abs(vy) < 0.005f) vy = 0;*/

		Input::Update();
	}
};