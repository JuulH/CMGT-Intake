#include "UIText.h"

UIText::UIText(UIContainer::Anchor _anchor, Tmpl8::vec2 _relPos, std::string _text, Tmpl8::Pixel _color, int _size = 4) 
	: text(_text)
{
	anchor = _anchor;
	relPos = _relPos;
	color = _color;
	size = _size;
	width = text.length() * 6 * size;
	height = 6 * size;
}

// Draw the text to the buffer
void UIText::Draw(Tmpl8::Surface* screen, Tmpl8::vec2 position)
{
	screen->Print(text, position.x, position.y, color, size);
}

// Set a new text and mark to be updated
void UIText::SetText(std::string _text) {
	if (text != _text) {
		text = _text;
		width = text.length() * 6 * size;
		isDirty = true;
	}
}