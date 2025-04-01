#pragma once

#include "Matrix/vec/vec.hpp"

enum LightType
{
    POINT_LIGHT,
    DIRECTIONAL_LIGHT,
    SPOT_LIGHT,
    NONE
};

// maybe it could be transform into ambiant light ?
struct ALight
{
    ml::vec3 color;
    float intensity;
    LightType type;

    ALight(const ml::vec3 &color = ml::vec3(1, 1, 1), float intensity = 1)
    {
        type = NONE;
        this->color = color;
        this->intensity = intensity;
    }

    virtual ~ALight() {}
};