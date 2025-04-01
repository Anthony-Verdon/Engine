#pragma once

#include "Engine/3D/Lights/ALight.hpp"

struct SpotLight: public ALight
{
    ml::vec3 position;
    ml::vec3 direction;
    float cutOff;
    float outerCutOff;

    SpotLight(const ml::vec3 &color = ml::vec3(1, 1, 1), float intensity = 1, const ml::vec3 &position = ml::vec3(0, 0, 0), const ml::vec3 &direction = ml::vec3(0, -1, 0), float cutOff = 12.5, float outerCutOff = 17.5): ALight(color, intensity)
    {
        type = SPOT_LIGHT;
        this->position = position;
        this->direction = direction;
        this->cutOff = cutOff;
        this->outerCutOff = outerCutOff;

    }
};