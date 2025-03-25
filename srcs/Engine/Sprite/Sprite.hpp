#pragma once

#include <string>
#include <glm/glm.hpp>

struct Sprite
{
    std::string textureName;
    glm::vec2 textureSize; // nb sprite on x and y axis
    glm::vec2 spriteCoords; // depending of texture size
    glm::vec2 size;

    bool operator==(const Sprite& other) const
    {
        return (textureName == other.textureName 
            && textureSize == other.textureSize
            && spriteCoords == other.spriteCoords
            && size == other.size);
    }

    static Sprite none;
};