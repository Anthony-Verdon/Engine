#include "Engine/UI/Button/Button.hpp"
#include "Engine/UI/UI.hpp"
#include "Engine/WindowManager/WindowManager.hpp"
#include "Engine/Renderers/SpriteRenderer/SpriteRenderer.hpp"

UI::Button::Button()
{
    for (size_t i = 0; i < 3; i++)
        sprites[i] = Sprite::none;
}
UI::Button::Button(const std::array<Sprite, 3> &sprites)
{
    this->sprites = sprites;
}

UI::Button::~Button()
{
}

void UI::Button::SetHot(UIState* ui, UIID uiID)
{
    if (!(ui->active.ID > 0) && uiID.layer >= ui->hotThisFrame.layer)
        ui->hotThisFrame = uiID;
}

bool UI::Button::Draw(UIState *ui, size_t ID, const glm::vec2 &position, const glm::vec2 &size)
{
    bool result = false;

    if (IsActive(ui, ID))
    {
        if (!WindowManager::IsInputPressed(GLFW_MOUSE_BUTTON_1)) // not holding anymore
        {
            if (PointInRectangle(WindowManager::GetMousePosition(), position, size)) // still on the button
                result = true;

            SetInactive(ui);
        }
        SpriteRenderer::Draw(SpriteRenderDataBuilder().SetPosition(position).SetSize(size).SetSprite(sprites[ButtonAnimation::ACTIVE]).SetDrawAbsolute(true).Build());
    }
    else if (IsHot(ui, ID))
    {
        if (WindowManager::IsInputPressed(GLFW_MOUSE_BUTTON_1))
            SetActive(ui, {ID, ui->globalLayer});
        SpriteRenderer::Draw(SpriteRenderDataBuilder().SetPosition(position).SetSize(size).SetSprite(sprites[ButtonAnimation::HOT]).SetDrawAbsolute(true).Build());
    }
    else
    {
        SpriteRenderer::Draw(SpriteRenderDataBuilder().SetPosition(position).SetSize(size).SetSprite(sprites[ButtonAnimation::INACTIVE]).SetDrawAbsolute(true).Build());
    }
    
    if (PointInRectangle(WindowManager::GetMousePosition(), position, size))
        SetHot(ui, {ID, ui->globalLayer});

    return (result);
}