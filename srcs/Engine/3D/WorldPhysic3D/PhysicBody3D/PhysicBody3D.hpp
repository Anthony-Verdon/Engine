#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>

class PhysicBody3D
{
  public:
    PhysicBody3D() {}
    virtual ~PhysicBody3D() {}

    PhysicBody3D(const PhysicBody3D &instance) = delete;
    PhysicBody3D &operator=(const PhysicBody3D &instance) = delete;

    // for Ragdolls, maybe we will need to add a BodyID param
    virtual void OnWorldPhysicUpdated() {}
    virtual void OnContactAdded([[maybe_unused]] const JPH::ContactManifold &inManifold) {}
    virtual void OnContactPersisted([[maybe_unused]] const JPH::ContactManifold &inManifold) {}
    virtual void OnContactRemoved() {}
};