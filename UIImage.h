#pragma once
#include "UIElement.h"
#include "UIContainer.h"

class UIImage : public UIElement
{
public:
	UIImage(UIContainer::Anchor anchor, Tmpl8::vec2 relPos, Tmpl8::Sprite* sprite, float size);
	virtual void Draw(Tmpl8::Surface* screen, Tmpl8::vec2 position) override;
	void SetImage(Tmpl8::Sprite* sprite);
	void SetFrame(int frame);

private:
	Tmpl8::Sprite* sprite;
	float size;
};

