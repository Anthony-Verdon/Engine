#pragma once

#include <glm/glm.hpp>
#include <string>

enum class TileBehaviorType;

class ATileBehavior
{
    protected:
        TileBehaviorType behaviorType;

    public:
        ATileBehavior() {}
        virtual ~ATileBehavior() {}

        virtual void behavior(const std::string &tilemapName, const glm::vec2 &actionCoords) = 0;

        TileBehaviorType GetType() {return behaviorType; }

};