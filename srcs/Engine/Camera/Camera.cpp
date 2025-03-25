#include "Engine/Camera/Camera.hpp"
#include "Engine/RessourceManager/RessourceManager.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "Engine/WindowManager/WindowManager.hpp"
#include "Engine/Renderers/SpriteRenderer/SpriteRenderer.hpp"
#include "Engine/Renderers/LineRenderer/LineRenderer.hpp"
#include "Engine/Renderers/PolygonRenderer/PolygonRenderer.hpp"
#include "Engine/Renderers/CircleRenderer/CircleRenderer.hpp"

Camera::Camera()
{
    position = glm::vec2(0, 0);
    zoom = 100;
}

Camera::~Camera()
{

}

glm::vec2 Camera::GetPosition() const
{
    return (position);
}

void Camera::SetPosition(const glm::vec2 &position)
{
    this->position = position;
}

void Camera::Move(const glm::vec2 &amount)
{
    this->position += amount;
}

float Camera::GetZoom() const
{
    return (zoom);
}

void Camera::Zoom(float amount)
{
    zoom = glm::clamp(zoom + amount * zoomFactor, 50.0f, 200.0f);
}

void Camera::UpdateShaders() const
{
    float left = position.x - WindowManager::GetWindowWidth() / 2 * zoom / 100;
    float right = position.x + WindowManager::GetWindowWidth() / 2 * zoom / 100;
    float top = position.y - WindowManager::GetWindowHeight() / 2 * zoom / 100;
    float bottom = position.y + WindowManager::GetWindowHeight() / 2 * zoom / 100;
    glm::mat4 projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

    SpriteRenderer::SetProjectionMatRelative(projection); //@todo factorize renderers with a common class ARenderer
    LineRenderer::SetProjectionMatRelative(projection);
    CircleRenderer::SetProjectionMatRelative(projection);
    PolygonRenderer::SetProjectionMatRelative(projection);
}