#pragma once

#include "Matrix/Matrix.hpp"

constexpr float zoomFactor = 5;

class Camera
{
    private:
        ml::vec2 position;
        float zoom;

    public:
        Camera();
        ~Camera();

        ml::vec2 GetPosition() const;
        void SetPosition(const ml::vec2 &position);
        void Move(const ml::vec2 &amount);
        
        float GetZoom() const;
        void Zoom(float amount);
        void UpdateShaders() const;
};