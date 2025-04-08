#pragma once

#include "GlbParser/GlbParser.hpp"

class Animation3D
{
    public:
        Animation3D(const std::vector<Glb::Node> &nodes, const Glb::Animation &animation);
        ~Animation3D();

        void Reset();
        void Update();
        bool Ended() const { return (ended); }
        ml::mat4 GetNodeTransform(size_t node) const;
        float AnimationDuration() const { return (animationDuration); }

    private:
        Glb::Animation data;
        std::map<int, ml::mat4> nodesTransform;
        std::map<int, ml::mat4> nodesInitTransform;
        float timer;
        float animationDuration;
        bool ended;

        ml::vec3 CalculateLerp(const ml::vec3 &previousPoint, const ml::vec3 &nextPoint, float interpolation);
        ml::vec4 CalculateSlerp(const ml::vec4 &previousPoint, const ml::vec4 &nextPoint, float interpolation);
};