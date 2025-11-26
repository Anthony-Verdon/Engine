#pragma once

#include "Engine/UI/AComponent/AComponent.hpp"
#include "Engine/2D/Sprite/Sprite.hpp"
#include <string>

namespace UI
{
class UISprite : public AComponent
{
  private:
    Sprite sprite;
    ml::vec2 position;
    ml::vec4 color;

  public:
    UISprite() = delete;
    UISprite(const Sprite &sprite, const ml::vec2 &position, const ml::vec4 &color = ml::vec4(1, 1, 1, 1));
    ~UISprite();

    void Draw();
};
}; // namespace UI