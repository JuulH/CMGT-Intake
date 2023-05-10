#pragma once
#include "surface.h"
#include "VerticalGround.h"
#include "Ground.h"
#include <vector>
#include <time.h>

class Terrain
{
public:
	static void GenerateTerrain(int segments);
	static void SetHole(int segment);
	static void RandomHole(int lower, int upper);
	static void GenerateDecor();
	static void NextLevel();

	static void Draw(Tmpl8::Surface* screen, float deltaTime, bool allowTransition);

	static std::vector<Ground> groundSegments;
	static std::vector<VerticalGround> verticalSegments;
	static const Tmpl8::Pixel primaryColor = 0x005500;
	static const Tmpl8::Pixel secondaryColor = 0x004400;
	static const Tmpl8::Pixel sandColor = 0xd4ba3b;
	static const Tmpl8::Pixel bounceColor = 0x333333;
	static int segmentCount;
	static bool transitioning;
	static bool transitionDone;
	static float surfaceOffset;
private:
	static void RedrawBuffer(Tmpl8::Surface* buffer);
	static std::vector<Tmpl8::vec2> trees;
	static int activePage;
	static Tmpl8::Surface terrainSurfaceFront;
	static Tmpl8::Surface terrainSurfaceBack;
	static Tmpl8::Surface* activeSurface;
	static Tmpl8::Surface* inactiveSurface;
	static Tmpl8::Pixel background;
	static Tmpl8::Sprite flag;
	static Tmpl8::Sprite tree;
	static float transitionSpeed;
	static Tmpl8::vec2 flagPosition;
	static float transitionOffset;
	static int amountType;
};