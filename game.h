#pragma once
#include "input.h"

namespace Tmpl8 {

class Surface;
class Game
{
public:
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Tick( float deltaTime );
private:
	Surface* screen;
};

}; // namespace Tmpl8