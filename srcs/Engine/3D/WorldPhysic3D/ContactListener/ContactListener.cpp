#include "Engine/3D/WorldPhysic3D/ContactListener/ContactListener.hpp"
#include <Jolt/Physics/Body/Body.h>

JPH::ValidateResult WorldPhysic3D::ContactListener::OnContactValidate(const JPH::Body &inBody1, const JPH::Body &inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult &inCollisionResult)
{
    (void)inBody1;
    (void)inBody2;
    (void)inBaseOffset;
    (void)inCollisionResult;

    return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

void WorldPhysic3D::ContactListener::OnContactAdded(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings)
{
    (void)ioSettings;

    WorldPhysic3D::bodies[inBody1.GetID()]->OnContactAdded(inManifold);
    WorldPhysic3D::bodies[inBody2.GetID()]->OnContactAdded(inManifold);
}

void WorldPhysic3D::ContactListener::OnContactPersisted(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings)
{
    (void)ioSettings;

    WorldPhysic3D::bodies[inBody1.GetID()]->OnContactPersisted(inManifold);
    WorldPhysic3D::bodies[inBody2.GetID()]->OnContactPersisted(inManifold);
}

void WorldPhysic3D::ContactListener::OnContactRemoved(const JPH::SubShapeIDPair &inSubShapePair)
{
    WorldPhysic3D::bodies[inSubShapePair.GetBody1ID()]->OnContactRemoved();
    WorldPhysic3D::bodies[inSubShapePair.GetBody2ID()]->OnContactRemoved();
}