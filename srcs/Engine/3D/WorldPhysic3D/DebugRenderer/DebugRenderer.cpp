#include <Jolt/Jolt.h>
#include "Engine/3D/WorldPhysic3D/DebugRenderer/DebugRenderer.hpp"
#include "Engine/3D/LineRenderer3D/LineRenderer3D.hpp"

void WorldPhysic3D::DebugRenderer::DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor)
{
    JPH::Vec4 jphColor = inColor.ToVec4();
    LineRenderer3D::Draw(ml::vec3(inFrom.GetX(), inFrom.GetY(), inFrom.GetZ()), ml::vec3(inTo.GetX(), inTo.GetY(), inTo.GetZ()), ml::vec4(jphColor.GetX(), jphColor.GetY(), jphColor.GetZ(), jphColor.GetW()));
}
void WorldPhysic3D::DebugRenderer::DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow)
{
    (void)inCastShadow;

    ml::vec3 v1 = ml::vec3(inV1.GetX(), inV1.GetY(), inV1.GetZ());
    ml::vec3 v2 = ml::vec3(inV2.GetX(), inV2.GetY(), inV2.GetZ());
    ml::vec3 v3 = ml::vec3(inV3.GetX(), inV3.GetY(), inV3.GetZ());
    ml::vec4 color = ml::vec4(inColor.r, inColor.g, inColor.b, inColor.a);
    LineRenderer3D::Draw(v1, v2, color);
    LineRenderer3D::Draw(v1, v3, color);
    LineRenderer3D::Draw(v2, v3, color);
}
void WorldPhysic3D::DebugRenderer::DrawText3D(JPH::RVec3Arg inPosition, const JPH::string_view &inString, JPH::ColorArg inColor, float inHeight)
{
    (void)inPosition;
    (void)inString;
    (void)inColor;
    (void)inHeight;

    std::cout << "DrawText3D" << std::endl;
}