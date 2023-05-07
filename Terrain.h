#pragma once
#include "surface.h"
#include "VerticalGround.h"
#include "Ground.h"
#include <vector>
#include <time.h>

namespace Tmpl8{

class Terrain
{
public:
	static void GenerateTerrain(int segments);
	static void SetHole(int segment);
	static void RandomHole(int lower, int upper);
	static void GenerateDecor();
	static void NextLevel();

	static void Draw(Tmpl8::Surface* screen, float deltaTime);

	static std::vector<Ground> groundSegments;
	static std::vector<VerticalGround> verticalSegments;
	static const Pixel primaryColor = 0x005500;
	static const Pixel secondaryColor = 0x003300;
	static int segmentCount;
	static bool transitioning;
	static bool transitionDone;
	static int surfaceOffset;
private:
	static void RedrawBuffer(Tmpl8::Surface* buffer);
	static std::vector<vec2> trees;
	static int activePage;
	static Surface terrainSurfaceFront;
	static Surface terrainSurfaceBack;
	static Surface* activeSurface;
	static Surface* inactiveSurface;
	static Pixel background;
	static Sprite flag;
	static Sprite tree;
	static float transitionSpeed;
	static vec2 flagPosition;
	static int transitionOffset;
};

}

