#pragma once

#include <glm/glm.hpp>

namespace UI
{
    bool PointInRectangle(const glm::vec2 &mousePosition, const glm::vec2 &rectPosition, const glm::vec2 &rectSize);

};