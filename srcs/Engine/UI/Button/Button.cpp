#include "Engine/UI/Button/Button.hpp"
#include "Engine/2D/Renderers/TextRenderer/TextRenderer.hpp"
#include "Engine/2D/Renderers/SpriteRenderer/SpriteRenderer.hpp"
#include "Engine/2D/Renderers/LineRenderer2D/LineRenderer2D.hpp"
#include "Engine/WindowManager/WindowManager.hpp"
#include "Engine/UI/UI.hpp"
#include "Engine/UI/Events.hpp"

UI::Button::Button(const std::string &text, const std::string &font, const ml::vec2 &pos, const ml::vec2 &size)
{
    this->text = text;
    this->font = font;
    this->pos = pos;
    this->size = size;

    hover = false;
    clicked = false;
    disable = false;
}

UI::Button::~Button()
{
}

void UI::Button::Update()
{
    if (disable)
        return;

    if (UI::PointInRectangle(WindowManager::GetMousePosition(), pos, size))
    {
        if (!hover)
        {
            EventData data(CURSEUR_ON);
            SendEvent(data);
            hover = true;
        }

        if (WindowManager::IsInputPressed(GLFW_MOUSE_BUTTON_1))
        {
            EventData data(CLICK_ON);
            SendEvent(data);
            clicked = true;
        }
        else if (clicked && WindowManager::IsInputReleased(GLFW_MOUSE_BUTTON_1))
        {
            EventData data(CLICK_OFF);
            SendEvent(data);
            clicked = false;
        }
    }
    else if (hover)
    {
        EventData data(CURSEUR_OFF);
        SendEvent(data);
        hover = false;
    }
}

void UI::Button::Draw()
{
    ml::vec4 color = ml::vec4(1, 1, 1, 1);
    if (disable)
        color = ml::vec4(0.5, 0.5, 0.5, 1);

    ml::vec2 leftCorner = pos - size / 2;
    LineRenderer2D::Draw(leftCorner, leftCorner + ml::vec2(size.x, 0), color, true);
    LineRenderer2D::Draw(leftCorner, leftCorner + ml::vec2(0, size.y), color, true);
    LineRenderer2D::Draw(leftCorner + size, leftCorner + ml::vec2(size.x, 0), color, true);
    LineRenderer2D::Draw(leftCorner + size, leftCorner + ml::vec2(0, size.y), color, true);
    LineRenderer2D::Draw(leftCorner, leftCorner + size, color, true);
    TextRenderer::Draw(text, font, leftCorner.x, leftCorner.y + size.y / 2, 1, color);
}

UI::SpriteButton::SpriteButton(const Sprite &sprite, const std::string &text, const std::string &font, const ml::vec2 &pos) : Button(text, font, pos, sprite.size), sprite(sprite)
{
}

UI::SpriteButton::~SpriteButton()
{
}

void UI::SpriteButton::Draw()
{
    ml::vec4 color = ml::vec4(1, 1, 1, 1);
    if (disable)
        color = ml::vec4(0.5, 0.5, 0.5, 1);
    SpriteRenderer::Draw(SpriteRenderDataBuilder().SetPosition(pos).SetSprite(sprite).SetSize(size).SetDrawAbsolute(true).SetColor(color).Build());
    TextRenderer::Draw(text, font, pos.x, pos.y, 1, color, TextRenderer::TextAlign::CENTER);
}