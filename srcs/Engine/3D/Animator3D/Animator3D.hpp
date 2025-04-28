#pragma once

#include <map>
#include "Engine/3D/Animation3D/Animation3D.hpp"
#include "GlbParser/GlbParser.hpp"

class Animator3D
{
    public:
        Animator3D();
        ~Animator3D();

        void LoadAnimation(const std::vector<Glb::Node> &nodes, const Glb::Animation &animation);
        
        void Play(const std::string &animation, bool replayAnimation = true, float animationSpeed = 1.0f);
        void Update();
        ml::mat4 GetNodeTransform(size_t node) const;
        std::vector<std::string> GetAnimationsName() const;
        bool CurrentAnimationEnded() const;
        float CurrentAnimationDuration() const;
        std::string GetCurrentAnimation() const { return currentAnimation; }

        bool IsNodeAnimated(int node) const;
    private:
        std::map<std::string, Animation3D> animations;
        std::string currentAnimation;
};