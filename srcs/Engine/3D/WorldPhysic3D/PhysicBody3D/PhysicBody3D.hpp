#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Collision/ContactListener.h>

class WorldPhysic3D;

class PhysicBody3D
{
  protected:
    int physicBodyType;

  public:
    PhysicBody3D() {}
    virtual ~PhysicBody3D() {}

    PhysicBody3D(const PhysicBody3D &instance) = delete;
    PhysicBody3D &operator=(const PhysicBody3D &instance) = delete;

    virtual void OnWorldPhysicUpdated() {}
    virtual void OnContactAdded([[maybe_unused]] const JPH::ContactManifold &inManifold, [[maybe_unused]] const PhysicBody3D *collisionedBody) {}
    virtual void OnContactPersisted([[maybe_unused]] const JPH::ContactManifold &inManifold, [[maybe_unused]] const PhysicBody3D *collisionedBody) {}
    virtual void OnContactRemoved([[maybe_unused]] const PhysicBody3D *collisionedBody) {}

    JPH::BodyID GetID() const { return (id); }
    int GetPhysicBodyType() const { return physicBodyType; }

  private:
    JPH::BodyID id;

    friend WorldPhysic3D;
};