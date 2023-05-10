#include "Ball.h"
#include "input.h"
#include "surface.h"
#include "game.h"

// Handle mouse input to shoot the ball
void Ball::HandleInput(Tmpl8::Surface* screen) {
	mousePosition = Input::GetMousePosition();
	ballMoving = v.length() > 0.03f;

	if (!ballMoving) {
		if (Input::GetMouseButtonDown(SDL_BUTTON_LEFT)) {
			mouseStart = mousePosition;
			startedAiming = true;
		}

		else if (Input::GetMouseButton(SDL_BUTTON_LEFT) && startedAiming) {
			mouseEnd = mousePosition;
			direction = mouseEnd - mouseStart;
			if (direction.length() > 300) direction = direction.normalized() * 300;
		}

		else if (Input::GetMouseButtonUp(SDL_BUTTON_LEFT) && startedAiming) {
			if (direction.length() > radius + 6) {
				direction *= -0.009f * (1 / (1 + 0.07f * (frictionFactor - 1)));
				v.x = direction.x;
				v.y = direction.y;

				startedAiming = false;
				strokes++;
				totalStrokes++;
				ballMoving = true;
				bounceCollisionCount = 0;
			}
			else {
				direction = Tmpl8::vec2(0, 0);
				startedAiming = false;
			}
		}
	}
}

void Ball::Update(float deltaTime) {
	// Acceleration/Friction
	a.x = -v.x * friction * frictionFactor;
	a.y = -v.y * friction * frictionFactor + gravity;

	// Velocity
	v.x += a.x * deltaTime;
	v.y += a.y * deltaTime;

	// Position
	pos.x += v.x * deltaTime;
	pos.y += v.y * deltaTime;

	// Reset ball if it falls off the screen
	if (pos.x > ScreenWidth + radius || pos.x < -radius) Reset();

	// Handle collisions
	activeGroundId = -1;

	for (VerticalGround& vg : Terrain::verticalSegments) {
		VerticalCollisions(vg);
	}

	// Only check current, previous and next segment for ground collisions
	activeSegment = (int)floorf(pos.x / (ScreenWidth / Terrain::segmentCount));
	GroundCollisions(Terrain::groundSegments[Tmpl8::Min<int>(Terrain::segmentCount - 1, Tmpl8::Max<int>(0, activeSegment - 1))]);
	GroundCollisions(Terrain::groundSegments[Tmpl8::Min<int>(Terrain::segmentCount - 1, Tmpl8::Max<int>(0, activeSegment))]);
	GroundCollisions(Terrain::groundSegments[Tmpl8::Min<int>(Terrain::segmentCount - 1, Tmpl8::Max<int>(0, activeSegment + 1))]);

	// Ground-specific interactions
	if (activeGroundId == 2) { // Sand
		frictionFactor = 6;
	}
	else {
		frictionFactor = 1;
	}
}

// Now unused, basic screen collisions
void Ball::ScreenCollisions() {
	if (pos.x < radius) {
		pos.x = radius;
		v.x = -v.x * bounciness * bounceFactor;
	}
	if (pos.x > ScreenWidth - radius) {
		pos.x = ScreenWidth - radius;
		v.x = -v.x * bounciness * bounceFactor;
	}
	if (pos.y < radius) {
		pos.y = radius;
		v.y = -v.y * bounciness * bounceFactor;
	}
	if (pos.y > ScreenHeight - radius) {
		pos.y = ScreenHeight - radius;
		v.y = -v.y * bounciness * bounceFactor;
	}
}

/// <summary>
/// Collisions with ground segments at angles.
/// Rotates the reference system to make the ground segment horizontal, simplyfying the calculations.
/// </summary>
/// <remarks>Adapted from Keith Peters' HTML5 Animation with JavaScript</remarks>
/// <param name="g">Ground segment to check for collisions</param>
void Ball::GroundCollisions(Ground g) {
	//if(pos.x - radius > g.end.x || pos.x + radius < g.start.x) return; // Not needed anymore as we only check the current, previous and next segment
	float upperY = Tmpl8::Min<int>(g.start.y, g.end.y);
	float lowerY = Tmpl8::Max<int>(g.start.y, g.end.y);
	if (abs(g.angle) > Tmpl8::PI * .1f && (pos.y + radius - 6 < upperY || pos.y - radius + 6 > lowerY)) return; // Crude fix for faulty collisions

	float dx = pos.x - g.middle.x;
	float dy = pos.y - g.middle.y;

	float cosine = cos(g.angle);
	float sine = sin(g.angle);

	float dx_ = cosine * dx + sine * dy;
	float dy_ = cosine * dy - sine * dx;

	if (dy_ > -radius && pos.x + radius >= g.start.x && pos.x - radius < g.end.x) {
		float velx_ = cosine * v.x + sine * v.y;
		float vely_ = -sine * v.x + cosine * v.y;

		activeGroundId = g.id;
		dy_ = -radius;

		if (activeGroundId == 2) { // Sand
			bounceFactor = 0.6f;
		}
		else if (activeGroundId == 3) { // Gel
			bounceCollisionCount++;
			bounceFactor = 2 * (1 / (1 + 0.1f * (bounceCollisionCount - 1))); // Prevent infinite bounces
		}
		else {
			bounceFactor = 1;
		}

		vely_ *= -bounciness * bounceFactor;

		dx = cosine * dx_ - sine * dy_;
		dy = cosine * dy_ + sine * dx_;

		v.x = cosine * velx_ - sine * vely_;
		v.y = cosine * vely_ + sine * velx_;

		pos.x = g.middle.x + dx;
		pos.y = g.middle.y + dy;
	}
}

// Vertical line collisions
void Ball::VerticalCollisions(VerticalGround g) {
	if(pos.x + radius < g.start.x || pos.x - radius > g.end.x) return;
	float upperY = Tmpl8::Min<int>(g.start.y, g.end.y);
	float lowerY = Tmpl8::Max<int>(g.start.y, g.end.y);
	if (pos.y + radius < upperY || pos.y - radius > lowerY) return;

	if (v.x > 0 && !g.left) {
		activeGroundId = g.id;
		pos.x = g.start.x - radius;
		v.x = -v.x * bounciness * bounceFactor;
	} else if (v.x < 0 && g.left) {
		activeGroundId = g.id;
		pos.x = g.end.x + radius;
		v.x = -v.x * bounciness * bounceFactor;
	}
}

void Ball::Reset() {
	pos.x = startX;
	pos.y = 350;
	v.x = .05, v.y = 0;
	a.x = -.02, a.y = gravity;
	activeGroundId = 0;
}

void Ball::Draw(Tmpl8::Surface* screen) {
	//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 2 });
	//printf("Ball:\nx: %f, y: %f\nvx: %f, vy: %f\nax: %f, ay: %f\np: %f\n\n\nStrokes: %i", pos.x, pos.y, v.x, v.y, a.x, a.y, abs((pos - previous).length()), strokes);
	
	if (startedAiming) screen->Line(pos.x, pos.y, pos.x - direction.x, pos.y - direction.y, 0xffffff);
	ballSprite.DrawScaled(pos.x - radius - Terrain::surfaceOffset, pos.y - radius, 26, 26, screen);
}