#pragma once

#include "Engine/2D/Sprite/Sprite.hpp"
#include "Matrix/Matrix.hpp"
#include <vector>

struct MTOBoundingBox;
struct MTOBoundingBoxBehavior;

// MTO stands for "MultiTileObject"
// this struct define what is the MTO at the first point:
// a sprite, a size (== how many tiles the object take),
// and bounding boxs which can have differents behaviors (collisions, trigger, ...)
struct MTO
{
    Sprite sprite;
    ml::vec2 size;
    std::vector<MTOBoundingBox> boundingBoxs;

    bool operator==(const MTO &other)
    {
        return (sprite == other.sprite && size == other.size);
    }

    static MTO none;
};

// a MTOInstance is an instance of a MTO in the world
// it's define by a position and the index of a MTO in the MTODictionnary
// and behaviors corresponding to each bounding box
struct MTOInstance
{
    ml::vec2 position;
    size_t index;
    std::vector<MTOBoundingBoxBehavior> behaviors;
};

struct MTOBoundingBox
{
    ml::vec2 position;
    ml::vec2 size;
};

struct MTOBoundingBoxBehavior
{
};
