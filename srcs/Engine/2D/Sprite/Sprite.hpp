#pragma once

#include <string>
#include "vec/vec.hpp"

struct Sprite
{
    std::string textureName;
    ml::vec2 textureSize; // nb sprite on x and y axis
    ml::vec2 spriteCoords; // depending of texture size
    ml::vec2 size;

    bool operator==(const Sprite& other) const
    {
        return (textureName == other.textureName 
            && textureSize == other.textureSize
            && spriteCoords == other.spriteCoords
            && size == other.size);
    }

    static Sprite none;
};