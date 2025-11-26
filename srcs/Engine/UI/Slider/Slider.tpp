#include "Engine/UI/Slider/Slider.hpp"
#include "Engine/2D/Renderers/SpriteRenderer/SpriteRenderer.hpp"
#include "Engine/UI/UI.hpp"
#include "Engine/WindowManager/WindowManager.hpp"
#include "Engine/macros.hpp"

template <typename T>
UI::Slider<T>::Slider(const Sprite &boundarySprite, const Sprite &pipeSprite, const Sprite &thumbSprite, T min, T max, T defaultValue, const ml::vec2 &pos, const ml::vec2 &size) : AComponent(pos), min(min), max(max), value(defaultValue), size(size)
{
    this->boundarySprite = boundarySprite;
    this->pipeSprite = pipeSprite;
    this->thumbSprite = thumbSprite;
    thumbPos = pos;
    clicked = false;
    leftBoundaryPos = pos + ml::vec2((-size.x + size.y) / 2, 0);
    rightBoundaryPos = pos + ml::vec2((size.x - size.y) / 2, 0);
    CHECK((rightBoundaryPos.x != leftBoundaryPos.x), "slider left and right boundary are on the same position, value won't change");
    CHECK((value >= min && value <= max), "value out of boundaries at init: value = " + std::to_string(value) + " | min = " + std::to_string(min) + " | max = " + std::to_string(max));
    value = std::clamp(value, min, max);
}

template <typename T>
UI::Slider<T>::~Slider()
{
}

template <typename T>
void UI::Slider<T>::Update()
{
    if (UI::PointInRectangle(WindowManager::GetMousePosition(), pos, size))
    {
        if (WindowManager::IsInputPressed(GLFW_MOUSE_BUTTON_1))
            clicked = true;
    }

    if (WindowManager::IsInputReleased(GLFW_MOUSE_BUTTON_1))
        clicked = false;
    else if (clicked)
    {
        thumbPos.x = std::clamp(WindowManager::GetMousePosition().x, leftBoundaryPos.x, rightBoundaryPos.x);
        if (rightBoundaryPos.x != leftBoundaryPos.x)
        {
            value = (thumbPos.x - leftBoundaryPos.x) / (rightBoundaryPos.x - leftBoundaryPos.x) * (max - min) + min;
            CHECK((value >= min && value <= max), "value out of boundaries: value = " + std::to_string(value) + " | min = " + std::to_string(min) + " | max = " + std::to_string(max));
            UpdateValueEventData data(value);
            SendEvent(data);
        }
    }
}

template <typename T>
void UI::Slider<T>::Draw()
{
    SpriteRenderer::Draw(SpriteRenderDataBuilder().SetSprite(boundarySprite).SetSize(ml::vec2(size.y, size.y)).SetDrawAbsolute(true).SetPosition(leftBoundaryPos).Build());
    SpriteRenderer::Draw(SpriteRenderDataBuilder().SetSprite(boundarySprite).SetSize(ml::vec2(size.y, size.y)).SetDrawAbsolute(true).SetPosition(rightBoundaryPos).FlipHorizontally(true).Build());
    SpriteRenderer::Draw(SpriteRenderDataBuilder().SetSprite(pipeSprite).SetSize(ml::vec2(size.x - size.y * 2, size.y)).SetDrawAbsolute(true).SetPosition(pos).Build());
    SpriteRenderer::Draw(SpriteRenderDataBuilder().SetSprite(thumbSprite).SetSize(thumbSprite.size).SetDrawAbsolute(true).SetPosition(thumbPos).Build());
}
