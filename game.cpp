#include "game.h"
#include "surface.h"
#include "template.h"
#include "input.h"
#include "ball.h"
#include "ground.h"
#include "Terrain.h"
#include "UIContainer.h"
#include "UIElement.h"
#include "Button.h"
#include "UIText.h"
#include "UIImage.h"
#include "SDL_events.h"
#include <cstdio> //printf
#include <time.h>
#include "UIParagraph.h"

namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	Ball b;

	enum GameState {
		MainMenu,
		Playing,
		Paused,
		GameOver,
	};

	GameState gameState;

	void SetState(GameState state) {
		gameState = state;
	}

	int levelCount = 0;

	/* ----------------------- UI -----------------------------------*/
	Sprite pauseSprite = Sprite(new Surface("assets/ui_pause.png"), 3);
	Sprite restartSprite = Sprite(new Surface("assets/ui_restart.png"), 3);
	Sprite resumeSprite = Sprite(new Surface("assets/ui_resume.png"), 3);
	Sprite quitSprite = Sprite(new Surface("assets/ui_quit.png"), 3);
	Sprite playSprite = Sprite(new Surface("assets/ui_play.png"), 3);
	Sprite logoSprite = Sprite(new Surface("assets/logo-banner.png"), 1);
	Sprite menuSprite = Sprite(new Surface("assets/ui_menu.png"), 3);
	Sprite windowSprite = Sprite(new Surface("assets/ui_window.png"), 1);
	Sprite helpSprite = Sprite(new Surface("assets/ui_help.png"), 3);
	Sprite closeSprite = Sprite(new Surface("assets/ui_close.png"), 3);

	bool pausedGame = false;
	void TogglePause() {
		pausedGame = !pausedGame;
		if (pausedGame) {
			SetState(Paused);
		}
		else {
			SetState(Playing);
		}
	}

	UIContainer* gameMenu;
	Button pauseButton(UIContainer::BOTTOM_LEFT, vec2(20, -20), &pauseSprite, .75f, TogglePause);
	UIText strokesText(UIContainer::TOP_CENTER, vec2(0, 50), "Strokes: 0", 0xffffffff, 6);
	UIText levelText(UIContainer::TOP_CENTER, vec2(0, 20), "Level 1", 0xffffffff, 4);
	UIText fpsText(UIContainer::TOP_LEFT, vec2(5, 5), "FPS: 0", 0xffffffff, 2);

	void Restart() {
		Terrain::GenerateTerrain(16);
		Terrain::RandomHole(Terrain::segmentCount / 2 + 1, Terrain::segmentCount - 1);
		b.Reset();
		b.strokes = 0;
		levelCount = 0;

		strokesText.SetText("Strokes: 0");
		levelText.SetText("Level 1");
		pausedGame = false;
		SetState(Playing);
	}

	UIContainer* gameOverMenu;
	UIText gameOverText(UIContainer::CENTER, vec2(0, -160), "Game Over!", 0xffffffff, 10);
	UIText gameOverLevelText(UIContainer::CENTER, vec2(0, -90), "You reached level 0", 0xffffffff, 6);
	UIText gameOverStrokesText(UIContainer::CENTER, vec2(0, -50), "in 0 strokes!", 0xffffffff, 4);
	Button restartButton(UIContainer::CENTER, vec2(0, 50), &restartSprite, 1, Restart);
	SDL_Event quitEvent{ SDL_QUIT };
	Button menuButton2(UIContainer::CENTER, vec2(0, 150), &menuSprite, 1, []() {SetState(MainMenu); });
	Button quitButton(UIContainer::CENTER, vec2(0, 250), &quitSprite, 1, []() {SDL_PushEvent(&quitEvent);});

	UIContainer* pauseMenu;
	UIText pauseText(UIContainer::CENTER, vec2(0, -180), "Paused", 0xffffffff, 9);
	Button resumeButton(UIContainer::CENTER, vec2(0, -50), &resumeSprite, 1, TogglePause);
	Button menuButton(UIContainer::CENTER, vec2(0, 50), &menuSprite, 1, []() {SetState(MainMenu); });
	Button quitButton2(UIContainer::CENTER, vec2(0, 150), &quitSprite, 1, []() {SDL_PushEvent(&quitEvent);});

	UIContainer* mainMenu;
	UIImage logo(UIContainer::CENTER, vec2(0, -150), &logoSprite, .7f);
	Button playButton(UIContainer::CENTER, vec2(0, 50), &playSprite, 1, []() {Restart();});
	UIImage helpWindow(UIContainer::CENTER, vec2(0, 0), &windowSprite, 1);
	Button quitButton3(UIContainer::CENTER, vec2(0, 250), &quitSprite, 1, []() {SDL_PushEvent(&quitEvent); });

	std::string h_text =
		"The goal of the game is simple: score the ball into the hole within 3 strokes, and keep going for as long as you can in an endlessly generated golf course!"
		" || To shoot the ball, click and hold down anywhere on the screen, and drag your mouse to aim. The ball will shoot in the direction of the line."
		" || The different ground types can be of help or work in your disadvantage: sand will increase friction and gel increases the bounciness."
		" || Thank you for playing my game!"
		" | JuulH, 2023.";
	UIParagraph helpText(UIContainer::CENTER, vec2(0, 0), h_text, 0xffffffff, 3, 650, 1);
	
	Button helpButton(UIContainer::CENTER, vec2(0, 150), &helpSprite, 1, []() {});
	Button closeButton(UIContainer::CENTER, vec2(0, 280), &closeSprite, 1, []() {});
	void ToggleHelp() {
		bool active = helpWindow.active;
		helpWindow.active = !active;
		closeButton.active = !active;
		helpText.active = !active;

		playButton.active = active;
		helpButton.active = active;
		quitButton3.active = active;
		mainMenu->ForceRedraw();
	}

	Sprite tiling(new Surface("assets/tiling.png"), 1);
	Surface tileBackground = Surface(ScreenWidth + ScreenWidth / 7, ScreenWidth + ScreenWidth / 7);
	float txOffset = 0;
	float tyOffset = 0;

	/* ----------------------- END UI -----------------------------------*/

	void Game::Init()
	{
		helpButton.SetFunc(ToggleHelp);
		closeButton.SetFunc(ToggleHelp);
		SetState(MainMenu);

		Terrain::GenerateTerrain(16);
		Terrain::RandomHole(Terrain::segmentCount / 2 + 1, Terrain::segmentCount - 1);
		b.Reset();

		// Set UIElements to UIContainers
		helpWindow.active = false;
		closeButton.active = false;
		helpText.active = false;

		gameMenu = new UIContainer(ScreenWidth, ScreenHeight, {
			&pauseButton,
			&strokesText,
			&levelText,
			&fpsText,
		});

		gameOverMenu = new UIContainer(ScreenWidth, ScreenHeight, {
			&gameOverText,
			&gameOverLevelText,
			&gameOverStrokesText,
			&restartButton,
			&quitButton,
			&fpsText,
			&menuButton2
		});

		pauseMenu = new UIContainer(ScreenWidth, ScreenHeight, {
			&pauseButton,
			&pauseText,
			&resumeButton,
			&quitButton2,
			&fpsText,
			&menuButton
		});

		mainMenu = new UIContainer(ScreenWidth, ScreenHeight, {
			&logo,
			&playButton,
			&quitButton3,
			&helpButton,
			&helpWindow,
			&closeButton,
			&helpText,
			&fpsText
		});

		tiling.DrawScaled(0, 0, ScreenWidth + ScreenWidth / 7, ScreenWidth + ScreenWidth / 7, &tileBackground);
	}
	
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown() {}

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	int averageFps = 0;
	float elapsedTime = 0;

	void Game::Tick(float deltaTime)
	{
		// Calculate average FPS
		elapsedTime += deltaTime;
		if (elapsedTime > 1000) {
			elapsedTime = 0;
			averageFps = (int)(1 / deltaTime * 1000);
			fpsText.SetText("FPS: " + std::to_string(averageFps));
		}

		vec2 mousePosition = Input::GetMousePosition();

		// Game state machine
		switch (gameState) {
		case MainMenu:
			// Scrolling background
			txOffset += deltaTime * .025f;
			tyOffset -= deltaTime * .025f;
			if (txOffset > 182) txOffset = 0;
			if (tyOffset < -182) tyOffset = 0;
			tileBackground.CopyTo(screen, static_cast<int>(txOffset - 183), static_cast<int>(tyOffset));

			mainMenu->Update(deltaTime, mousePosition);
			mainMenu->Draw(screen);
			break;

		case Playing:
			// Skip level
			if (Input::GetKeyDown(SDL_SCANCODE_P)) {
				Terrain::NextLevel();
			}

			if (Terrain::transitionDone) b.Reset();
			if (!Terrain::transitioning) {
				if (!(gameMenu->ignoreInput && !b.startedAiming)) b.HandleInput(screen);
				strokesText.SetText("Strokes: " + std::to_string(b.strokes) + "/3");

				b.Update(deltaTime);

				// Check if ball is in hole
				if (b.activeGroundId == 1) {
					Terrain::NextLevel();
					b.strokes = 0;
					levelCount++;
					levelText.SetText("Level " + std::to_string(levelCount));
				}
				
				// Max 3 strokes per level
				else if (!b.ballMoving && b.strokes >= 3) {
					SetState(GameOver);
					gameOverLevelText.SetText("You reached level " + std::to_string(levelCount));
					gameOverStrokesText.SetText("in " + std::to_string(b.totalStrokes) + " strokes!");
					gameOverMenu->ForceRedraw();
				}
			}

			Terrain::Draw(screen, deltaTime, true);
			b.Draw(screen);
			gameMenu->Update(deltaTime, mousePosition);
			gameMenu->Draw(screen);
			break;

		case Paused:
			Terrain::Draw(screen, deltaTime, false);
			b.Draw(screen);
			pauseMenu->Update(deltaTime, mousePosition);
			pauseMenu->Draw(screen);
			break;

		case GameOver:
			// Scrolling background
			txOffset += deltaTime * .025f;
			tyOffset -= deltaTime * .025f;
			if (txOffset > 182) txOffset = 0;
			if (tyOffset < -182) tyOffset = 0;
			tileBackground.CopyTo(screen, static_cast<int>(txOffset - 183), static_cast<int>(tyOffset));

			gameOverMenu->Update(deltaTime, mousePosition);
			gameOverMenu->Draw(screen);
			break;
		}

		Input::Update();
	}
};