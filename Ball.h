#pragma once
#include "surface.h"
#include "template.h"
#include "Ground.h"
#include "VerticalGround.h"
#include "Terrain.h"

namespace Tmpl8 {
	class Surface;

class Ball
{
public:
	void HandleInput(Tmpl8::Surface* screen);
	void Update(float deltaTime);
	void ScreenCollisions();
	void GroundCollisions(Ground g);
	void VerticalCollisions(VerticalGround v);
	void Reset();
	void Draw(Tmpl8::Surface* screen, Sprite* sprite);
	int strokes = 0;
	int activeGroundId = 0;
	int startX = -13;
	vec2 pos = vec2(ScreenWidth / 8, 0);
	vec2 v;
private:
	float ScreenWidth = 1280;
	float ScreenHeight = 720;
	float gravity = 0.0035f;
	vec2 a = vec2(0, gravity);
	float bounciness = 0.6f;
	float friction = 0.001f;
	bool startedAiming = false;
	float radius = 13;
	vec2 mousePosition;
	vec2 mouseStart;
	vec2 mouseEnd;
	vec2 direction;
	vec2 previous = vec2(9999, 9999);
	bool ignoreBounce = false;
};


}