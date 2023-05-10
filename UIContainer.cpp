#include "UIContainer.h"

UIContainer::UIContainer(int _width, int _height, std::initializer_list<UIElement*> _elements)
{
	width = _width;
	height = _height;
	elements = _elements;
	UIBuffer = new Tmpl8::Surface(width, height);
	UIBuffer->Clear(0x00000000);

	for (UIElement* element : elements)
	{
		element->pos = ComputePosition(Anchor(element->anchor), element->relPos, element->width, element->height);
		if (!element->active) continue;
		element->Draw(UIBuffer, element->pos);
	}
}

void UIContainer::AddElement(UIElement* element)
{
	elements.push_back(element);
}

void UIContainer::AddElement(UIElement element)
{
	elements.push_back(&element);
}

void UIContainer::AddElements(std::initializer_list<UIElement*> _elements)
{
	elements.insert(elements.end(), _elements.begin(), _elements.end());
}

//void UIContainer::RemoveElement(UIElement* element)
//{
//	elements.erase(std::remove(elements.begin(), elements.end(), *element), elements.end());
//}

//void UIContainer::RemoveElement(UIElement element)
//{
//	//elements.erase(std::remove(elements.begin(), elements.end(), element), elements.end());
//}

void UIContainer::Draw(Tmpl8::Surface* screen)
{
	for (UIElement* element : elements)
	{
		if (element->isDirty && element->active)
		{
			Tmpl8::vec2 pos = ComputePosition(Anchor(element->anchor), element->relPos, element->width, element->height);
			UIBuffer->Clear(0x00000000, pos.x, pos.y, element->width, element->height);
			element->Draw(UIBuffer, pos);
			element->isDirty = false;
		}
	}
	UIBuffer->CopyToAlpha(screen, 0, 0);
}

void UIContainer::Update(float deltaTime, Tmpl8::vec2 mousePosition)
{
	ignoreInput = false;
	for (UIElement* element : elements)
	{
		if (!element->active) continue;
		bool isUsing = element->HandleInput(mousePosition);
		if (isUsing) ignoreInput = true;
	}
}

void UIContainer::ForceRedraw() {
	UIBuffer->Clear(0x00000000);
	for (UIElement* element : elements)
	{
		if (element->active)
		{
			Tmpl8::vec2 pos = ComputePosition(Anchor(element->anchor), element->relPos, element->width, element->height);
			element->Draw(UIBuffer, pos);
			element->isDirty = false;
		}
	}
}

/// <summary>
///  Compute the position of an element based on it's anchor, relative position and dimensions.
/// </summary>
/// <param name="anchor">Screen alignment</param>
/// <param name="relPos">Offset from anchor position</param>
/// <param name="e_width">Element width</param>
/// <param name="e_height">Element height</param>
/// <returns></returns>
Tmpl8::vec2 UIContainer::ComputePosition(Anchor anchor, Tmpl8::vec2 relPos, float e_width, float e_height) {
	// Calculate position based on anchor and width of element
	int x = 0, y = 0;
	switch (anchor) {
		case TOP_LEFT: break;
		case TOP_CENTER: x = (width / 2) - (e_width / 2); break;
		case TOP_RIGHT: x = width - e_width; break;

		case CENTER_LEFT: y = (height / 2) - (e_height / 2); break;
		case CENTER: x = (width / 2) - (e_width / 2); y = (height / 2) - (e_height / 2); break;
		case CENTER_RIGHT: x = width - e_width; y = (height / 2) - (e_height / 2); break;

		case BOTTOM_LEFT: y = height - e_height; break;
		case BOTTOM_CENTER: x = (width / 2) - (e_width / 2); y = height - e_height; break;
		case BOTTOM_RIGHT: x = width - e_width; y = height - e_height; break;

	}
	return relPos + Tmpl8::vec2(x, y);
}