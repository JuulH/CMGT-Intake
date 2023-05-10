#include "UIParagraph.h"

UIParagraph::UIParagraph(UIContainer::Anchor _anchor, Tmpl8::vec2 _relPos, std::string _text, Tmpl8::Pixel _color, int _size = 4, int _maxWidth = 400, int lineSpacing = 1)
    : text(_text)
{
	anchor = _anchor;
	relPos = _relPos;
	color = _color;
	size = _size;
    maxWidth = _maxWidth;
    width = static_cast<float>(maxWidth);
	lineHeight = (6 + lineSpacing) * size;
    height = static_cast<float>(lineHeight);
	SplitLines();
}

// Iterate over the lines and draw them under another
void UIParagraph::Draw(Tmpl8::Surface* screen, Tmpl8::vec2 position)
{
    for (int i = 0; i < lines.size(); i++)
    {
        screen->Print(lines[i], static_cast<int>(position.x), static_cast<int>(position.y + (i * lineHeight)), color, size);
    }
}

void UIParagraph::SetText(std::string _text) {
	if (text != _text) {
		text = _text;
        SplitLines();
	}
}

/// <summary>
/// Split a string into multiple lines to fill the width of the element.
/// Use '|' as a newline character.
/// </summary>
void UIParagraph::SplitLines() {
    lines.clear();
    if (!text.empty()) {
        std::istringstream iss(text);
        std::string line;
        while (std::getline(iss, line))
        {
            std::istringstream lineStream(line);
            std::string word;
            std::string lineContent;
            while (lineStream >> word) {
                if (word == "||") {
                    lines.push_back(lineContent);
                    lineContent.clear();
                    lines.push_back(lineContent);
                    continue;
                }
                else if (word == "|") {
                    lines.push_back(lineContent);
                    lineContent.clear();
                    continue;
                }

                if (lineContent.empty())
                {
                    lineContent = word;
                }
                else if (lineContent.length() + word.length() + 1 <= maxWidth / (6 * size))
                {
                    lineContent += " " + word;
                }
                else
                {
                    lines.push_back(lineContent);
                    lineContent = word;
                }
            }
            if (!lineContent.empty())
            {
                lines.push_back(lineContent);
            }
        }
    }
    width = static_cast<float>(maxWidth);
    height = static_cast<float>(lineHeight * lines.size());
    isDirty = true;
}