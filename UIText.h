#pragma once
#include "UIElement.h"
#include "UIContainer.h"
#include <string>

class UIText : public UIElement
{
public:
	UIText(UIContainer::Anchor anchor, Tmpl8::vec2 relPos, std::string text, Tmpl8::Pixel color, int size);
	virtual void Draw(Tmpl8::Surface* screen, Tmpl8::vec2 position) override;
	void SetText(std::string text);

private:
	std::string text;
	Tmpl8::Pixel color;
	int size;
};

