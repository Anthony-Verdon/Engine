#pragma once

#include "Engine/3D/Lights/ALight.hpp"

struct DirectionalLight: public ALight
{
    ml::vec3 direction;

    DirectionalLight(const ml::vec3 &color = ml::vec3(1, 1, 1), float intensity = 1, const ml::vec3 &direction = ml::vec3(0, -1, 0)): ALight(color, intensity)
    {
        type = DIRECTIONAL_LIGHT;
        this->direction = direction;
    }
};