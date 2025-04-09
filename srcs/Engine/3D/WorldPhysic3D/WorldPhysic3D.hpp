#pragma once

#include <memory>
#include <Jolt/Jolt.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

class WorldPhysic3D
{
    public:
        static void Init(const JPH::BroadPhaseLayerInterface &BPLayerInterface, const JPH::ObjectVsBroadPhaseLayerFilter &objVsBPLayerFilter, const JPH::ObjectLayerPairFilter &OLPFilter);
        static void Update();
        static void DebugDraw(const JPH::BodyManager::DrawSettings &inSettings, JPH::DebugRenderer *inRenderer, const JPH::BodyDrawFilter *inBodyFilter = nullptr);
        static void Destroy();

        static JPH::BodyInterface &GetBodyInterface() { return bodyInterface; }

        static void AddBodyActivationListener(JPH::BodyActivationListener *listener) { physicSystem.SetBodyActivationListener(listener); }
        static void SetContactListener(JPH::ContactListener *listener) { physicSystem.SetContactListener(listener); }
    private:
        WorldPhysic3D() = delete;

        static std::unique_ptr<JPH::TempAllocatorImpl> tempAllocator;
        static std::unique_ptr<JPH::JobSystemThreadPool> jobSystem;
        static JPH::PhysicsSystem physicSystem;
    	static JPH::BodyInterface &bodyInterface;
};