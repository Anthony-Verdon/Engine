#pragma once

#include "Engine/2D/Sprite/Sprite.hpp"
#include "Engine/2D/ATileBehavior/ATileBehavior.hpp"
#include "Matrix/Matrix.hpp"
#include <vector>
#include <memory>

struct Tile
{
    Sprite sprite;
    ml::vec2 spriteOffset;
    ml::vec2 boundingBox;
    std::vector<TileBehaviorType> behaviors;

    bool operator==(const Tile &other) const
    {
        return (sprite == other.sprite && spriteOffset == other.spriteOffset && boundingBox == other.boundingBox);
    }

    static Tile none;
};