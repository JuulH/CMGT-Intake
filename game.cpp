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
	void Game::Tick(float deltaTime)
	{
		// Example of input implementation
		vec2 mousePosition = Input::GetMousePosition();

		if (Input::GetKeyDown(SDLK_q)) printf("Q Down\n");
		if (Input::GetKey(SDLK_q)) printf("Q Held\n");
		if (Input::GetKeyUp(SDLK_q)) printf("Q Up\n");

		if (Input::GetMouseButtonDown(SDL_BUTTON_LEFT)) printf("Left Down\n");
		if (Input::GetMouseButton(SDL_BUTTON_RIGHT)) printf("Right Held\n");
		if (Input::GetMouseButtonUp(SDL_BUTTON_MIDDLE)) printf("Middle Up\n");

		Input::Update();
	}
};