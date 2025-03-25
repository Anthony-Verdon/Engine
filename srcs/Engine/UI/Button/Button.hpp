#pragma once

#include <glm/glm.hpp>
#include <array>
#include "Engine/Sprite/Sprite.hpp"

namespace UI
{

    struct UIID
    {
        size_t ID;
        size_t layer;
    };
    
    struct UIState
    {
        UIID active;
        UIID hotThisFrame;
        UIID hotLastFrame;

        size_t globalLayer;
    };

    class Button
    {
        private:
            enum ButtonAnimation
            {
                INACTIVE,
                HOT, // hovering
                ACTIVE // clicked + holding
            };

            std::array<Sprite, 3> sprites;

            void SetHot(UIState* ui, UIID uiID);
            void SetActive(UIState* ui, UIID uiID) { ui->active = uiID; }
            void SetInactive(UIState* ui) { ui->active = {}; }
            
            bool IsHot(UIState* ui, size_t ID) { return (ui->hotLastFrame.ID > 0 && ui->hotLastFrame.ID == ID); }
            bool IsActive(UIState* ui, size_t ID) { return (ui->active.ID > 0 && ui->active.ID == ID); }        
            
            public:
            Button();
            Button(const std::array<Sprite, 3> &sprites);
            ~Button();
            
            void SetSprite(const std::array<Sprite, 3> &sprites) {this->sprites = sprites; }
            
            bool Draw(UIState *ui, size_t ID, const glm::vec2 &position, const glm::vec2 &size);
    };
};