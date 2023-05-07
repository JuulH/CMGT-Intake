#include "game.h"
#include "surface.h"
#include "template.h"
#include "input.h"
#include "ball.h"
#include "ground.h"
#include "Terrain.h"
#include "Button.h"
#include <cstdio> //printf
#include <time.h>

namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------


	Ball b;
	Pixel background = 0x4466ff;
	Surface backgroundSurf(ScreenWidth, ScreenHeight);
	void Game::Init()
	{
		
		Terrain::GenerateTerrain(16);
		Terrain::RandomHole(Terrain::segmentCount / 2 + 1, Terrain::segmentCount - 1);
		b.Reset();

		backgroundSurf.Clear(background);
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
	int levelCount = 0;
	char strokesText[14];
	char levelText[11];
	char fpsText[10];
	int averageFps = 0;
	float elapsedTime = 0;
	float fpsUpdateTime = 1000; // ms
	bool enterDown = false;
	char debugText[20];
	int activeSegment = 0;
	bool paused = false;
	void TogglePause() {
		paused = !paused;
	}
	Sprite buttonSprite = Sprite(new Surface("assets/ui_pause_sheet.png"), 3);
	Button pauseButton = Button(vec2(0, 0), vec2(50, 50), &buttonSprite, .25f, TogglePause);

	void Game::Tick(float deltaTime)
	{
		// Calculate average FPS
		elapsedTime += deltaTime;
		if (elapsedTime > 1000) {
			elapsedTime = 0;
			averageFps = (int)(1 / deltaTime * 1000);
		}

		if (Input::GetKeyDown(SDL_SCANCODE_P)) {
			Terrain::NextLevel();
			//b.Reset();
		}

		if (Input::GetMouseButtonDown(SDL_BUTTON_RIGHT)) {
			b.pos = Input::GetMousePosition();
			b.v = vec2(0, 0);
		}

		/*screen->Clear(background);*/
		//backgroundSurf.CopyTo(screen, 0, 0);

		Terrain::Draw(screen, deltaTime);

		if (Terrain::transitionDone) b.Reset();
		if (!Terrain::transitioning) {
			if(!pauseButton.HandleInput(Input::GetMousePosition())) b.HandleInput(screen);
			b.Update(deltaTime);
			activeSegment = (int)floorf(b.pos.x / (ScreenWidth / Terrain::segmentCount));
			sprintf(debugText, "i: %i", activeSegment);
			//screen->Print(debugText, b.pos.x - 20, b.pos.y - 25, 0xffffff, 2);

			// Collisions TODO: Move class? And reference instead of copy
			for (VerticalGround& vg : Terrain::verticalSegments) {
				b.VerticalCollisions(vg);
				//screen->Print(vg.left ? "left" : "right", vg.start.x, vg.start.y, 0xffffff, 2);
			}

			b.GroundCollisions(Terrain::groundSegments[Min<int>(Terrain::segmentCount - 1, Max<int>(0, activeSegment - 1))]);
			b.GroundCollisions(Terrain::groundSegments[Min<int>(Terrain::segmentCount - 1, Max<int>(0, activeSegment))]);
			b.GroundCollisions(Terrain::groundSegments[Min<int>(Terrain::segmentCount - 1, Max<int>(0, activeSegment + 1))]);

			//b.ScreenCollisions();

			// Ground-specific interactions
			// 0: None, 1: Hole
			if (b.activeGroundId == 1) {
				Terrain::NextLevel();
				/*Terrain::GenerateTerrain(16);
				Terrain::RandomHole(Terrain::segmentCount / 2 + 1, Terrain::segmentCount - 1);
				Terrain::GenerateDecor();*/
				levelCount++;
			}
		}


		sprintf(strokesText, "Strokes: %d", b.strokes);
		screen->Centre(strokesText, 50, 0xffffff, 6);

		sprintf(levelText, "Level %i", levelCount);
		screen->Centre(levelText, 20, 0xffffff, 4);

		sprintf(fpsText, "FPS: %i", averageFps);
		screen->Print(fpsText, 5, 5, 0xffffff, 2);

		// Add button
		pauseButton.Draw(screen);

		b.Draw(screen, &ball);

		//vx *= pow(0.9994f, deltaTime);
		//vy *= pow(0.997f, deltaTime);

		/*if (abs(vx) < 0.005f) vx = 0;
		if (abs(vy) < 0.005f) vy = 0;*/

		Input::Update();
	}
};