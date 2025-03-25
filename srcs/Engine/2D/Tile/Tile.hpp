#pragma once

#include "Engine/2D/Sprite/Sprite.hpp"
#include "Engine/2D/ATileBehavior/ATileBehavior.hpp"
#include "vec/vec.hpp"
#include <vector>
#include <memory>

struct Tile
{
    Sprite sprite;
    ml::vec2 spriteOffset;
    std::vector<TileBehaviorType> behaviors;

    bool operator==(const Tile& other) const
    {
        return (sprite == other.sprite && spriteOffset == other.spriteOffset);
    }

    static Tile none;
};