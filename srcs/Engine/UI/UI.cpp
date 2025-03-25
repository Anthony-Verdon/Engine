#include "Engine/UI/UI.hpp"

bool UI::PointInRectangle(const glm::vec2 &mousePosition, const glm::vec2 &rectPosition, const glm::vec2 &rectSize)
{
    glm::vec2 HalftRectSize = rectSize / 2.0f;
    return (mousePosition.x >= rectPosition.x - HalftRectSize.x
        && mousePosition.y >= rectPosition.y - HalftRectSize.x
        && mousePosition.x <= rectPosition.x + HalftRectSize.x 
        && mousePosition.y <= rectPosition.y + HalftRectSize.y);
}