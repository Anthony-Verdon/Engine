#pragma once

#include "Engine/3D/Lights/ALight.hpp"

struct PointLight: public ALight
{
    ml::vec3 position;

    PointLight(const ml::vec3 &color = ml::vec3(1, 1, 1), float intensity = 1, const ml::vec3 &position = ml::vec3(0, 0, 0)): ALight(color, intensity)
    {
        type = POINT_LIGHT;
        this->position = position;
    }
};