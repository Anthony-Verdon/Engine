#pragma once

#include "Engine/3D/WorldPhysic3D/PhysicBody3D/PhysicBody3D.hpp"
#include <Jolt/Physics/Ragdoll/Ragdoll.h>

class WorldPhysic3D;

class Ragdoll : public PhysicBody3D
{
  public:
    Ragdoll() {}
    virtual ~Ragdoll() {}

    const JPH::Array<JPH::BodyID> &GetBodyIDs() const { return ragdoll->GetBodyIDs(); }

  private:
    JPH::Ref<JPH::Ragdoll> ragdoll;

    friend WorldPhysic3D;
};