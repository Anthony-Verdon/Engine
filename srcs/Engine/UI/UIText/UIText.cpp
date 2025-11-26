#include "Engine/UI/UIText/UIText.hpp"

UI::UIText::UIText(const std::string &text, const std::string &font, const ml::vec2 &position, TextRenderer::TextAlign textAlign)
{
    this->text = text;
    this->font = font;
    pos = position;
    this->textAlign = textAlign;
}

UI::UIText::~UIText()
{
}

void UI::UIText::Draw()
{
    TextRenderer::Draw(text, font, pos.x, pos.y, 1, ml::vec4(1, 1, 1, 1), textAlign);
}