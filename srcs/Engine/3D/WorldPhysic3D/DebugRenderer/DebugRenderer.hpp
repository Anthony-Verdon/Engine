#pragma once

#include "Engine/3D/WorldPhysic3D/WorldPhysic3D.hpp"
#include <Jolt/Renderer/DebugRendererSimple.h>

class WorldPhysic3D::DebugRenderer : public JPH::DebugRendererSimple
{
  public:
    virtual void DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor);
    virtual void DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow = ECastShadow::Off);
    virtual void DrawText3D(JPH::RVec3Arg inPosition, const JPH::string_view &inString, JPH::ColorArg inColor = JPH::Color::sWhite, float inHeight = 0.5f);
};