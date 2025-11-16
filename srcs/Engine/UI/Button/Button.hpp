#pragma once

#include "Engine/UI/AComponent/AComponent.hpp"
#include <string>

namespace UI
{
class Button : public AComponent
{
  private:
    std::string text;
    std::string font;
    ml::vec2 size;
    ml::vec4 color;

    bool clicked;
    bool hover;

  public:
    Button();
    Button(const std::string &text, const std::string &font, const ml::vec2 &pos, const ml::vec2 &size);
    ~Button();

    void Update();
    void Draw();
};
}; // namespace UI