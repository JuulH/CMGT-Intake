#pragma once
#include "surface.h"
#include "template.h"
#include "UIElement.h"
#include <vector>
#include <Windows.h>

class UIContainer
{
public:
	UIContainer(int width, int height, std::initializer_list<UIElement*> elements);
	void AddElement(UIElement* element);
	void AddElement(UIElement element);
	void AddElements(std::initializer_list<UIElement*> elements);
	//void RemoveElement(UIElement* element);
	//void RemoveElement(UIElement element);
	void Draw(Tmpl8::Surface* screen);
	void Update(float deltaTime, Tmpl8::vec2 mousePosition);
	void ForceRedraw();

	static enum Anchor {
		TOP_LEFT,
		TOP_CENTER,
		TOP_RIGHT,
		CENTER_LEFT,
		CENTER,
		CENTER_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_CENTER,
		BOTTOM_RIGHT
	};

	bool ignoreInput = false; // Game should ignore input when interacting with UI
private:
	Tmpl8::Surface* UIBuffer;
	float width;
	float height;
	std::vector<UIElement*> elements;
	Tmpl8::vec2 ComputePosition(Anchor anchor, Tmpl8::vec2 relPos, float e_width, float e_height);
};