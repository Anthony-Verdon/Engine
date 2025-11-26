#pragma once

#include "Engine/UI/AComponent/AComponent.hpp"
#include "Engine/2D/Renderers/TextRenderer/TextRenderer.hpp"
#include <string>

namespace UI
{
class UIText : public AComponent
{
  private:
    std::string text;
    std::string font;
    TextRenderer::TextAlign textAlign;

  public:
    UIText() = delete;
    UIText(const std::string &text, const std::string &font, const ml::vec2 &position, TextRenderer::TextAlign textAlign);
    ~UIText();

    void UpdateText(const std::string &text) { this->text = text; }
    void Draw();
};
}; // namespace UI