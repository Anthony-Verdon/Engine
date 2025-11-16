#pragma once

#include "Matrix/Matrix.hpp"

namespace UI
{
bool PointInRectangle(const ml::vec2 &mousePosition, const ml::vec2 &rectPosition, const ml::vec2 &rectSize);
};