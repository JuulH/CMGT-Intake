#include "Terrain.h"

namespace Tmpl8 {

	std::vector<Ground> Terrain::groundSegments;
	std::vector<VerticalGround> Terrain::verticalSegments;
	std::vector<vec2> Terrain::trees;
	int Terrain::segmentCount;
	vec2 Terrain::flagPosition;
	int Terrain::activePage = 0;
	Surface Terrain::terrainSurfaceFront(ScreenWidth, ScreenHeight);
	Surface Terrain::terrainSurfaceBack(ScreenWidth, ScreenHeight);
	Surface* Terrain::activeSurface = &terrainSurfaceFront;
	Surface* Terrain::inactiveSurface = &terrainSurfaceBack;
	Pixel Terrain::background = 0x4466ff;
	Sprite Terrain::flag(new Surface("assets/flag.png"), 1);
	Sprite Terrain::tree(new Surface("assets/tree.png"), 1);
	bool Terrain::transitioning = false;
	int Terrain::surfaceOffset = 0;
	float Terrain::transitionSpeed = 0.9f;
	bool Terrain::transitionDone = false;
	int Terrain::transitionOffset = 0;

	void Terrain::GenerateTerrain(int segments) {
		RedrawBuffer(inactiveSurface); // ??

		groundSegments.clear();
		segmentCount = segments;

		float segmentWidth = ScreenWidth / segments;
		float startY = ScreenHeight - 300;

		srand(time(NULL)); // Seed random number generator
		for (int i = 0; i < segments; i++) {
			// Randomize endY within limits
			float endY = startY + rand() % 200 - 100;
			if (endY > 720 - 150) endY -= 150;
			if (endY < 150) endY += 150;

			Pixel color = i % 2 == 0 ? primaryColor : secondaryColor;
			groundSegments.push_back(Ground(vec2(segmentWidth * i, startY), vec2(segmentWidth * (i + 1), endY), color));
			startY = endY;
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

	void Terrain::SetHole(int holeSegment) {

		verticalSegments.clear();

		// Get halfway point of hole segment
		float holeX = (groundSegments[holeSegment].start.x + groundSegments[holeSegment].end.x) / 2;
		float holeY = (groundSegments[holeSegment].start.y + groundSegments[holeSegment].end.y) / 2;

		Ground holeBottom = Ground(vec2(holeX + -30, holeY + 70), vec2(holeX + 30, holeY + 70), 0xffffff, 1);

		flagPosition = holeBottom.middle;

		verticalSegments.push_back(VerticalGround(vec2(holeX + -30, holeY), vec2(holeX + -30, holeY + 70), 0xffffff, true));
		verticalSegments.push_back(VerticalGround(vec2(holeX + 30, holeY + 70), vec2(holeX + 30, holeY), 0xffffff, false));

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
				Pixel color = i % 2 == 0 ? Terrain::primaryColor : Terrain::secondaryColor;
				vec2 segmentMiddle = groundSegments[i].middle;
				trees.push_back(vec2(segmentMiddle.x - 300 / 8, segmentMiddle.y - 559 / 4 + 10));
			}
		}
	}

	float EaseInOut(float t) {
		if (t < 0.5) {
			return 0.5 * pow(2 * t, 2);
		}
		else {
			return -0.5 * ((2 * t - 2) * (2 * t - 2) - 2);
		}
	}

	void Terrain::Draw(Surface* screen, float deltaTime) {
		if (!transitioning) activeSurface->CopyTo(screen, 0, 0),
			transitionDone = false;
		else {
			// Calculate the eased offset based on the transition progress
			float easedProgress = EaseInOut((float)transitionOffset / (float)ScreenWidth);
			surfaceOffset = easedProgress * ScreenWidth;

			inactiveSurface->CopyTo(screen, -surfaceOffset, 0);
			activeSurface->CopyTo(screen, ScreenWidth - surfaceOffset, 0);

			transitionOffset += deltaTime * transitionSpeed;

			if (transitionOffset >= ScreenWidth) {
				transitionOffset = 0;
				surfaceOffset = 0;
				transitioning = false;
				transitionDone = true;
			}
		}
	}

	void Terrain::RedrawBuffer(Surface* buffer) {
		buffer->Clear(background);
		for (vec2 t : trees) {
			tree.DrawScaledOverlay(t.x, t.y, 300 / 4, 559 / 4, primaryColor, buffer);
		}
		for (Ground g : Terrain::groundSegments) {
			g.Draw(buffer);
		}
		for (VerticalGround vg : Terrain::verticalSegments) {
			vg.Draw(buffer);
		}

		flag.DrawScaled(flagPosition.x - 10, flagPosition.y - 668 / 4, 256 / 4, 668 / 4, buffer);
	}

	void Terrain::NextLevel() {
		activePage = activePage == 0 ? 1 : 0;
		Surface* inactiveSurface = activeSurface;
		Surface* activeSurface = activePage == 0 ? &terrainSurfaceFront : &terrainSurfaceBack;

		GenerateTerrain(segmentCount);
		RandomHole(segmentCount / 2 + 1, segmentCount - 1);
		transitioning = true;
	}

}