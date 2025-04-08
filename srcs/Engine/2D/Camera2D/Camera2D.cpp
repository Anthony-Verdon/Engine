#include "Engine/2D/Camera2D/Camera2D.hpp"
#include "Engine/RessourceManager/RessourceManager.hpp"
#include "Engine/WindowManager/WindowManager.hpp"
#include "Engine/2D/Renderers/SpriteRenderer/SpriteRenderer.hpp"
#include "Engine/2D/Renderers/LineRenderer2D/LineRenderer2D.hpp"
#include "Engine/2D/Renderers/PolygonRenderer/PolygonRenderer.hpp"
#include "Engine/2D/Renderers/CircleRenderer/CircleRenderer.hpp"

Camera2D::Camera2D()
{
    position = ml::vec2(0, 0);
    zoom = 100;
}

Camera2D::~Camera2D()
{

}

ml::vec2 Camera2D::GetPosition() const
{
    return (position);
}

void Camera2D::SetPosition(const ml::vec2 &position)
{
    this->position = position;
}

void Camera2D::Move(const ml::vec2 &amount)
{
    this->position += amount;
}

float Camera2D::GetZoom() const
{
    return (zoom);
}

void Camera2D::Zoom(float amount)
{
    zoom = ml::clamp(zoom + amount * zoomFactor, 50.0f, 200.0f);
}

void Camera2D::UpdateShaders() const
{
    float left = position.x - WindowManager::GetWindowWidth() / 2 * zoom / 100;
    float right = position.x + WindowManager::GetWindowWidth() / 2 * zoom / 100;
    float top = position.y - WindowManager::GetWindowHeight() / 2 * zoom / 100;
    float bottom = position.y + WindowManager::GetWindowHeight() / 2 * zoom / 100;
    ml::mat4 projection = ml::ortho(left, right, bottom, top, -1.0f, 1.0f);

    SpriteRenderer::SetProjectionMatRelative(projection); //@todo factorize renderers with a common class ARenderer
    LineRenderer2D::SetProjectionMatRelative(projection);
    CircleRenderer::SetProjectionMatRelative(projection);
    PolygonRenderer::SetProjectionMatRelative(projection);
}