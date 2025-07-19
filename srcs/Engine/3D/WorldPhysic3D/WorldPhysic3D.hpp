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
  public:
    static void Init(const JPH::BroadPhaseLayerInterface &BPLayerInterface, const JPH::ObjectVsBroadPhaseLayerFilter &objVsBPLayerFilter, const JPH::ObjectLayerPairFilter &OLPFilter);
    static void Update();
    static void DebugDraw(const JPH::BodyManager::DrawSettings &inSettings = {}, const JPH::BodyDrawFilter *inBodyFilter = nullptr);
    static void Destroy();

    // todo: remove this method
    static JPH::BodyInterface &GetBodyInterface() { return bodyInterface; }
    static void AddBody(PhysicBody3D *ptr, const JPH::BodyCreationSettings &settings, JPH::EActivation inActivationMode);
    static void RemoveBody(const JPH::BodyID &id);

    static float GetDeltaTime() { return (deltaTime); }
    static void SetDeltaTime(float deltaTime) { WorldPhysic3D::deltaTime = deltaTime; }
    static int GetCollisionStep() { return (collisionStep); }
    static void SetCollisionStep(int collisionStep) { WorldPhysic3D::collisionStep = collisionStep; }

  private:
    WorldPhysic3D() = delete;

    class DebugRenderer;
    class ContactListener;
    friend ContactListener;

    static std::unique_ptr<JPH::TempAllocatorImpl> tempAllocator;
    static std::unique_ptr<JPH::JobSystemThreadPool> jobSystem;
    static ContactListener contactListener;
    static JPH::PhysicsSystem physicSystem;
    static JPH::BodyInterface &bodyInterface;

    static float deltaTime;
    static int collisionStep;
    static std::map<JPH::BodyID, PhysicBody3D *> bodies;
};