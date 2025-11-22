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

  public:
    UISprite() = delete;
    UISprite(const Sprite &sprite, const ml::vec2 &position);
    ~UISprite();

    void Draw();
};
}; // namespace UI