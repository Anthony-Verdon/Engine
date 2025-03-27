#pragma once

#include "Matrix/Matrix.hpp"
#include <string>

enum class TileBehaviorType;

class ATileBehavior
{
    protected:
        TileBehaviorType behaviorType;

    public:
        ATileBehavior() {}
        virtual ~ATileBehavior() {}

        virtual void behavior(const std::string &tilemapName, const ml::vec2 &actionCoords) = 0;

        TileBehaviorType GetType() {return behaviorType; }

};