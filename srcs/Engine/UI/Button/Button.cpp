#include "Engine/UI/Button/Button.hpp"
#include "Engine/2D/Renderers/TextRenderer/TextRenderer.hpp"
#include "Engine/2D/Renderers/LineRenderer2D/LineRenderer2D.hpp"
#include "Engine/WindowManager/WindowManager.hpp"
#include "Engine/UI/UI.hpp"
#include "Engine/UI/Events.hpp"

UI::Button::Button()
{
    hover = false;
    clicked = false;
}

UI::Button::Button(const std::string &text, const std::string &font, const ml::vec2 &pos, const ml::vec2 &size)
{
    this->text = text;
    this->font = font;
    this->pos = pos;
    this->size = size;

    hover = false;
    clicked = false;
}

UI::Button::~Button()
{
}

void UI::Button::Update()
{
    if (UI::PointInRectangle(WindowManager::GetMousePosition(), pos + size / 2, size))
    {
        if (!hover)
        {
            SendEvent(CURSEUR_ON);
            hover = true;
        }

        if (WindowManager::IsInputPressed(GLFW_MOUSE_BUTTON_1))
        {
            SendEvent(CLICK_ON);
            clicked = true;
        }
        else if (clicked && WindowManager::IsInputReleased(GLFW_MOUSE_BUTTON_1))
        {
            SendEvent(CLICK_OFF);
            clicked = false;
        }
    }
    else if (hover)
    {
        {
            SendEvent(CURSEUR_OFF);
            hover = false;
        }
    }
}

void UI::Button::Draw()
{
    LineRenderer2D::Draw(pos, pos + ml::vec2(size.x, 0), ml::vec3(1, 1, 1), true);
    LineRenderer2D::Draw(pos, pos + ml::vec2(0, size.y), ml::vec3(1, 1, 1), true);
    LineRenderer2D::Draw(pos + size, pos + ml::vec2(size.x, 0), ml::vec3(1, 1, 1), true);
    LineRenderer2D::Draw(pos + size, pos + ml::vec2(0, size.y), ml::vec3(1, 1, 1), true);
    TextRenderer::Draw(text, font, pos.x, pos.y, 1, ml::vec4(1, 1, 1, 1));
}