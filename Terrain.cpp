#include "Terrain.h"

std::vector<Ground> Terrain::groundSegments;
std::vector<VerticalGround> Terrain::verticalSegments;
int Terrain::segmentCount;
int Terrain::amountType = 4;

std::vector<Tmpl8::vec2> Terrain::trees;
Tmpl8::vec2 Terrain::flagPosition;
Tmpl8::Sprite Terrain::flag(new Tmpl8::Surface("assets/flag.png"), 1);
Tmpl8::Sprite Terrain::tree(new Tmpl8::Surface("assets/tree.png"), 1);

int Terrain::activePage = 0;
Tmpl8::Surface Terrain::terrainSurfaceFront(ScreenWidth, ScreenHeight);
Tmpl8::Surface Terrain::terrainSurfaceBack(ScreenWidth, ScreenHeight);
Tmpl8::Surface* Terrain::activeSurface = &terrainSurfaceFront;
Tmpl8::Surface* Terrain::inactiveSurface = &terrainSurfaceBack;
Tmpl8::Pixel Terrain::background = 0x4466ff;

bool Terrain::transitioning = false;
float Terrain::surfaceOffset = 0;
float Terrain::transitionSpeed = 0.8f;
bool Terrain::transitionDone = false;
float Terrain::transitionOffset = 0;

/// <summary>
/// Generate a new terrain with the given amount of segments.
/// </summary>
/// <param name="segments">The amount of segments</param>
void Terrain::GenerateTerrain(int segments) {
	RedrawBuffer(inactiveSurface); // ??

	groundSegments.clear();
	segmentCount = segments;

	float segmentWidth = static_cast<float>(ScreenWidth / segments);
	float startY = ScreenHeight - 300;

	srand(static_cast<int>(time(NULL))); // Seed random number generator
	for (int i = 0; i < segments; i++) {
		// Randomize endY within limits
		float endY = startY + rand() % 200 - 100;
		if (endY > 720 - 150) endY -= 150;
		if (endY < 150) endY += 150;

		Tmpl8::Pixel color = i % 2 == 0 ? primaryColor : secondaryColor;
		groundSegments.push_back(Ground(Tmpl8::vec2(segmentWidth * i, startY), Tmpl8::vec2(segmentWidth * (i + 1), endY), color));
		startY = endY;
	}

	// Random chance for other terrain types per level
	int typeLeft = amountType;
	int typeLen = 0;
	int randType = 0;
	for (int i = 0; i < segments; i++) {
		if (typeLeft == 0) break;
		if (typeLen > 0) {
			groundSegments[i].id = randType;
			groundSegments[i].color = randType == 2 ? sandColor : bounceColor;
			typeLen--;
			typeLeft--;
		}
		else if (rand() % 100 < 7) {
			int lower = 1;
			int upper = typeLeft;
			typeLen = lower + (rand() % (upper - lower + 1));

			lower = 2;
			upper = 3;
			randType = lower + (rand() % (upper - lower + 1));

			groundSegments[i].id = randType;
			groundSegments[i].color = randType == 2 ? sandColor : bounceColor;
			typeLen--;
			typeLeft--;
		}
	}

	// Make flat starting platform for ball to start on placed at the second segment
	float segmentMiddleY = groundSegments[1].middle.y;
	groundSegments[1].start.y = ScreenHeight - 275;
	groundSegments[1].end.y = ScreenHeight - 275;
	groundSegments[1].ComputeProperties();

	// Connect previous and next segments to the starting platform
	groundSegments[0].end.x = groundSegments[1].start.x;
	groundSegments[0].end.y = groundSegments[1].start.y;
	groundSegments[0].start.y = ScreenHeight - 290;
	groundSegments[0].ComputeProperties();

	groundSegments[2].start.x = groundSegments[1].end.x;
	groundSegments[2].start.y = groundSegments[1].end.y;
	groundSegments[2].ComputeProperties();

	groundSegments[segments - 1].end.y = ScreenHeight - 290;
	groundSegments[segments - 1].ComputeProperties();
}

/// <summary>
/// Replace given ground segment with the hole.
/// </summary>
/// <param name="holeSegment">The ground segment to replace</param>
void Terrain::SetHole(int holeSegment) {

	verticalSegments.clear();

	// Get halfway point of hole segment
	float holeX = (groundSegments[holeSegment].start.x + groundSegments[holeSegment].end.x) / 2;
	float holeY = (groundSegments[holeSegment].start.y + groundSegments[holeSegment].end.y) / 2;

	Ground holeBottom = Ground(Tmpl8::vec2(holeX + -30, holeY + 70), Tmpl8::vec2(holeX + 30, holeY + 70), 0xffffff, 1);

	flagPosition = holeBottom.middle;

	verticalSegments.push_back(VerticalGround(Tmpl8::vec2(holeX + -30, holeY), Tmpl8::vec2(holeX + -30, holeY + 70), 0xffffff, true));
	verticalSegments.push_back(VerticalGround(Tmpl8::vec2(holeX + 30, holeY + 70), Tmpl8::vec2(holeX + 30, holeY), 0xffffff, false));

	groundSegments[holeSegment - 1].end.x = verticalSegments[0].start.x - 1;
	groundSegments[holeSegment - 1].end.y = verticalSegments[0].start.y + 0;
	groundSegments[holeSegment - 1].ComputeProperties();

	groundSegments[holeSegment + 1].start.x = verticalSegments[1].end.x + 1;
	groundSegments[holeSegment + 1].start.y = verticalSegments[1].end.y + 0;
	groundSegments[holeSegment + 1].ComputeProperties();

	// Add hole array to groundSegments array
	groundSegments[holeSegment] = holeBottom;

	GenerateDecor();
	RedrawBuffer(activeSurface);
}

void Terrain::RandomHole(int lower, int upper) {
	int holeSegment = rand() % (upper - lower) + lower;
	SetHole(holeSegment);
}

void Terrain::GenerateDecor() {
	trees.clear();

	for (int i = 0; i < segmentCount; i++) {
		if(groundSegments[i].id != 0) continue;

		// Chance to add tree to middle of segment
		if (rand() % 100 < 10) {
			Tmpl8::Pixel color = i % 2 == 0 ? Terrain::primaryColor : Terrain::secondaryColor;
			Tmpl8::vec2 segmentMiddle = groundSegments[i].middle;
			trees.push_back(Tmpl8::vec2(segmentMiddle.x - 300 / 8, segmentMiddle.y - 559 / 4 + 10));
		}
	}
}

// Ease in-out modifier adapted from https://easings.net/#easeInOutQuad
float EaseInOut(float t) {
	if (t <= 0.5f) {
		return 2 * t * t;
	}
	else {
		t -= 0.5f;
		return 2 * t * (1 - t) + 0.5f;
	}
}

void Terrain::Draw(Tmpl8::Surface* screen, float deltaTime, bool allowTransition = true) {
	if (!transitioning) activeSurface->CopyTo(screen, 0, 0),
		transitionDone = false;
	else if (allowTransition){
		// Calculate the eased offset based on the transition progress
		float easedProgress = EaseInOut(transitionOffset / ScreenWidth);
		surfaceOffset = easedProgress * ScreenWidth;

		inactiveSurface->CopyTo(screen, static_cast<int>(-surfaceOffset), 0);
		activeSurface->CopyTo(screen, static_cast<int>(ScreenWidth - surfaceOffset), 0);

		transitionOffset += deltaTime * transitionSpeed;

		if (transitionOffset >= ScreenWidth) {
			transitionOffset = 0;
			surfaceOffset = 0;
			transitioning = false;
			transitionDone = true;
		}
	}
}

/// <summary>
/// Redraw the terrain buffer once when terrain changes.
/// </summary>
/// <param name="buffer">The surface buffer to draw to.</param>
void Terrain::RedrawBuffer(Tmpl8::Surface* buffer) {
	buffer->Clear(background);
	for (Tmpl8::vec2 t : trees) {
		tree.DrawScaledOverlay(static_cast<int>(t.x), static_cast<int>(t.y), 300 / 4, 559 / 4, primaryColor, buffer);
	}
	for (Ground g : Terrain::groundSegments) {
		g.Draw(buffer);
	}
	for (VerticalGround vg : Terrain::verticalSegments) {
		vg.Draw(buffer);
	}

	flag.DrawScaled(static_cast<int>(flagPosition.x - 10), static_cast<int>(flagPosition.y - 668 / 4), 256 / 4, 668 / 4, buffer);
}

void Terrain::NextLevel() {
	activePage = activePage == 0 ? 1 : 0;
	Tmpl8::Surface* inactiveSurface = activeSurface;
	Tmpl8::Surface* activeSurface = activePage == 0 ? &terrainSurfaceFront : &terrainSurfaceBack;

	GenerateTerrain(segmentCount);
	RandomHole(segmentCount / 2 + 1, segmentCount - 1);
	transitioning = true;
}