#include "Engine/UI/UISprite/UISprite.hpp"
#include "Engine/2D/Renderers/SpriteRenderer/SpriteRenderer.hpp"

UI::UISprite::UISprite(const Sprite &sprite, const ml::vec2 &position, const ml::vec4 &color)
{
    this->sprite = sprite;
    pos = position;
    this->color = color;
}

UI::UISprite::~UISprite()
{
}

void UI::UISprite::Draw()
{
    SpriteRenderer::Draw(SpriteRenderDataBuilder().SetColor(color).SetSprite(sprite).SetPosition(pos).SetSize(sprite.size).SetDrawAbsolute(true).Build());
}