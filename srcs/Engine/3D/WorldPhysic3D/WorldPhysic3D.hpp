#pragma once

#include <memory>
#include <Jolt/Jolt.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include "Engine/3D/WorldPhysic3D/ContactListener/ContactListener.hpp"

class WorldPhysic3D
{
  public:
    static void Init(const JPH::BroadPhaseLayerInterface &BPLayerInterface, const JPH::ObjectVsBroadPhaseLayerFilter &objVsBPLayerFilter, const JPH::ObjectLayerPairFilter &OLPFilter);
    static void Update();
    static void DebugDraw(const JPH::BodyManager::DrawSettings &inSettings = {}, const JPH::BodyDrawFilter *inBodyFilter = nullptr);
    static void Destroy();

    static JPH::BodyInterface &GetBodyInterface() { return bodyInterface; }

    static void AddBodyActivationListener(JPH::BodyActivationListener *listener) { physicSystem.SetBodyActivationListener(listener); }
    static JPH::ContactListener *GetContactListener() { return physicSystem.GetContactListener(); }

    static float GetDeltaTime() { return (deltaTime); }
    static void SetDeltaTime(float deltaTime) { WorldPhysic3D::deltaTime = deltaTime; }

  private:
    WorldPhysic3D() = delete;

    static std::unique_ptr<JPH::TempAllocatorImpl> tempAllocator;
    static std::unique_ptr<JPH::JobSystemThreadPool> jobSystem;
    static ContactListener contactListener;
    static JPH::PhysicsSystem physicSystem;
    static JPH::BodyInterface &bodyInterface;

    static float deltaTime;
};