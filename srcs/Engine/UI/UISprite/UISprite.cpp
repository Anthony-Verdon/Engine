#include "Engine/UI/UISprite/UISprite.hpp"
#include "Engine/2D/Renderers/SpriteRenderer/SpriteRenderer.hpp"

UI::UISprite::UISprite(const Sprite &sprite, const ml::vec2 &position)
{
    this->sprite = sprite;
    pos = position;
}

UI::UISprite::~UISprite()
{
}

void UI::UISprite::Draw()
{
    SpriteRenderer::Draw(SpriteRenderDataBuilder().SetSprite(sprite).SetPosition(pos).SetSize(sprite.size).SetDrawAbsolute(true).Build());
}