#pragma once
#include "UIElement.h"
#include "UIContainer.h"
#include <string>
#include <sstream>
#include <algorithm>

class UIParagraph : public UIElement
{
public:
	UIParagraph(UIContainer::Anchor anchor, Tmpl8::vec2 relPos, std::string text, Tmpl8::Pixel color, int size, int maxWidth, int lineSpacing);
	virtual void Draw(Tmpl8::Surface* screen, Tmpl8::vec2 position) override;
	void SetText(std::string text);

private:
	std::string text;
	std::vector<std::string> lines;
	Tmpl8::Pixel color;
	int size;
	int maxWidth;
	int lineHeight;

	void SplitLines();
	char newline = '|';
};

