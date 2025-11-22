#pragma once

#include "Engine/UI/AComponent/AComponent.hpp"
#include "Engine/2D/Sprite/Sprite.hpp"
#include <string>

namespace UI
{
class Button : public AComponent
{
  protected:
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

    void UpdateText(const std::string &text) { this->text = text; }

    void Update();
    void Draw();
};

class SpriteButton : public Button
{
  private:
    Sprite sprite;

  public:
    SpriteButton();
    SpriteButton(const Sprite &sprite, const std::string &text, const std::string &font, const ml::vec2 &pos);
    ~SpriteButton();

    void UpdateSprite(const Sprite &sprite) { this->sprite = sprite; }

    void Draw();
};
}; // namespace UI