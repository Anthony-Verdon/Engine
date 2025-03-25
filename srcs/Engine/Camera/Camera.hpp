#pragma once

#include <glm/glm.hpp>

constexpr float zoomFactor = 5;

class Camera
{
    private:
        glm::vec2 position;
        float zoom;

    public:
        Camera();
        ~Camera();

        glm::vec2 GetPosition() const;
        void SetPosition(const glm::vec2 &position);
        void Move(const glm::vec2 &amount);
        
        float GetZoom() const;
        void Zoom(float amount);
        void UpdateShaders() const;
};