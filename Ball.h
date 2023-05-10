#pragma once
#include "surface.h"
#include "template.h"
#include "Ground.h"
#include "VerticalGround.h"
#include "Terrain.h"

class Ball
{
public:
	Ball() : ballSprite(new Tmpl8::Surface("assets/golfball.png"), 1) {};
	void HandleInput(Tmpl8::Surface* screen);
	void Update(float deltaTime);
	void ScreenCollisions();
	void GroundCollisions(Ground g);
	void VerticalCollisions(VerticalGround v);
	void Reset();
	void Draw(Tmpl8::Surface* screen);

	int strokes = 0;
	int totalStrokes = 0;
	int activeGroundId = 0;

	int startX = -13;
	Tmpl8::vec2 pos = Tmpl8::vec2(ScreenWidth / 8, 0);
	Tmpl8::vec2 v;
	bool ballMoving = true;

	bool startedAiming = false;
	float frictionFactor = 1;
	float bounceFactor = 1;

private:
	float gravity = 0.0035f;
	float friction = 0.001f;
	float bounciness = 0.65f;
	float radius = 13;
	Tmpl8::vec2 a = Tmpl8::vec2(0, gravity);
	Tmpl8::Sprite ballSprite;

	Tmpl8::vec2 mousePosition;
	Tmpl8::vec2 mouseStart;
	Tmpl8::vec2 mouseEnd;
	Tmpl8::vec2 direction;

	int bounceCollisionCount = 0;
	int activeSegment = 0;
};