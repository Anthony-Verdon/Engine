#include "Engine/UI/UI.hpp"

bool UI::PointInRectangle(const ml::vec2 &mousePosition, const ml::vec2 &rectPosition, const ml::vec2 &rectSize)
{
    ml::vec2 HalftRectSize = rectSize / 2.0f;
    return (mousePosition.x >= rectPosition.x - HalftRectSize.x && mousePosition.y >= rectPosition.y - HalftRectSize.y && mousePosition.x <= rectPosition.x + HalftRectSize.x && mousePosition.y <= rectPosition.y + HalftRectSize.y);
}