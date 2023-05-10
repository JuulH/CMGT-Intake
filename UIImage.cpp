#include "UIImage.h"

UIImage::UIImage(UIContainer::Anchor _anchor, Tmpl8::vec2 _relPos, Tmpl8::Sprite* _sprite, float _size = 4)
{
	anchor = _anchor;
	relPos = _relPos;
	sprite = _sprite;
	size = _size;
	width = sprite->GetWidth() * size;
	height = sprite->GetHeight() * size;
}

void UIImage::Draw(Tmpl8::Surface* screen, Tmpl8::vec2 position)
{
	sprite->DrawScaled(pos.x, pos.y, width, height, screen);
}

void UIImage::SetImage(Tmpl8::Sprite* _sprite) {
	if (sprite != _sprite) {
		sprite = _sprite;
		width = sprite->GetWidth() * size;
		height = sprite->GetHeight() * size;
		isDirty = true;
	}
}

void UIImage::SetFrame(int frame) {
	if (sprite->Frames() > frame) {
		sprite->SetFrame(frame);
		isDirty = true;
	}
}