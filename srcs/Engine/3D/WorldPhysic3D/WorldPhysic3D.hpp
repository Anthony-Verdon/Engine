#pragma once

#include <map>
#include <memory>
#include <Jolt/Jolt.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/EActivation.h>
#include "Engine/3D/WorldPhysic3D/PhysicBody3D/PhysicBody3D.hpp"

class WorldPhysic3D
{
  private:
    class BroadPhaseLayerInterface;
    class ObjectVsBroadPhaseLayerFilter;
    class ObjectLayerPairFilter;
#if DEBUG_DRAW_PHYSIC_3D
    class DebugRenderer;
#endif
    class ContactListener;
    friend ContactListener;

  public:
    static void Init();
    static void Update();
    static void DebugDraw(const JPH::BodyManager::DrawSettings &inSettings = {}, const JPH::BodyDrawFilter *inBodyFilter = nullptr);
    static void Destroy();

    static void AddBody(PhysicBody3D *ptr, const JPH::BodyCreationSettings &settings, JPH::EActivation inActivationMode);
    static void RemoveBody(const JPH::BodyID &id);

    static float GetDeltaTime() { return (deltaTime); }
    static void SetDeltaTime(float deltaTime) { WorldPhysic3D::deltaTime = deltaTime; }
    static int GetCollisionStep() { return (collisionStep); }
    static void SetCollisionStep(int collisionStep) { WorldPhysic3D::collisionStep = collisionStep; }

    static void SetShape(const JPH::BodyID &inBodyID, const JPH::Shape *inShape, bool inUpdateMassProperties, JPH::EActivation inActivationMode) { bodyInterface.SetShape(inBodyID, inShape, inUpdateMassProperties, inActivationMode); }
    static JPH::Vec3 GetLinearVelocity(const JPH::BodyID &inBodyID) { return (bodyInterface.GetLinearVelocity(inBodyID)); }
    static void SetLinearVelocity(const JPH::BodyID &inBodyID, JPH::Vec3Arg inLinearVelocity) { bodyInterface.SetLinearVelocity(inBodyID, inLinearVelocity); }
    static JPH::Vec3 GetPosition(const JPH::BodyID &inBodyID) { return (bodyInterface.GetPosition(inBodyID)); }
    static void SetPosition(const JPH::BodyID &inBodyID, JPH::RVec3Arg inPosition, JPH::EActivation inActivationMode) { bodyInterface.SetPosition(inBodyID, inPosition, inActivationMode); }

    static BroadPhaseLayerInterface broadPhaseLayerInterface;
    static ObjectVsBroadPhaseLayerFilter objectVsBroadPhaseLayerFilter;
    static ObjectLayerPairFilter objectLayerPairFilter;

  private:
    WorldPhysic3D() = delete;

    static std::unique_ptr<JPH::TempAllocatorImpl> tempAllocator;
    static std::unique_ptr<JPH::JobSystemThreadPool> jobSystem;
    static ContactListener contactListener;
    static JPH::PhysicsSystem physicSystem;
    static JPH::BodyInterface &bodyInterface;

    static float deltaTime;
    static int collisionStep;
    static std::map<JPH::BodyID, PhysicBody3D *> bodies;
};