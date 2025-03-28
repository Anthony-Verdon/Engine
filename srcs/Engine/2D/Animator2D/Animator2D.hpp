#pragma once

#include <map>
#include <string>
#include "Engine/2D/Animation2D/Animation2D.hpp"

class Animator2D
{
    private:
        std::map<std::string, Animation2D> animations;
        std::string currentAnimation;

    public:
        Animator2D();
        ~Animator2D();

        void AddAnimation(const std::string &name, const Animation2D &animation);
        void Play(const std::string &name);
        void Update();
        Sprite GetFrame() const;
        bool CurrentAnimationEnded() const;
        std::string GetCurrentAnimation() const;
};