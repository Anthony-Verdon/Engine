#pragma once

#include "Engine/3D/WorldPhysic3D/PhysicBody3D/PhysicBody3D.hpp"
#include <Jolt/Physics/Body/Body.h>

class WorldPhysic3D;

class RigidBody : public PhysicBody3D
{
  public:
    RigidBody() {}
    virtual ~RigidBody() {}

    JPH::BodyID GetID() const { return (id); }

  private:
    JPH::BodyID id;

    friend WorldPhysic3D;
};