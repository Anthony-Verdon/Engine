#pragma once

#include "Engine/2D/Renderers/SpriteRenderer/SpriteRenderer.hpp"
#include "Matrix/Matrix.hpp"
#include <vector>

class Animation2D
{
    private:
        std::vector<Sprite> frames;
        float animationSpeed;
        float timer;
        unsigned int frameIndex;
        bool stoppable;
        bool ended;

    public:
        Animation2D(float animationSpeed = 0.1f, bool stoppable = true);
        ~Animation2D();

        void AddFrame(const Sprite &frame);
        void SetFrames(const std::vector<Sprite> &frames);
        void Update();
        void Reset();

        void SetAnimationSpeed(float animationSpeed);
        void SetStoppable(bool stoppable);

        bool IsStoppable() const;
        bool Ended() const;
        Sprite GetFrame() const;
        std::vector<Sprite> GetFrames() const;
        unsigned int GetFrameIndex() const;

        static Animation2D none;
};