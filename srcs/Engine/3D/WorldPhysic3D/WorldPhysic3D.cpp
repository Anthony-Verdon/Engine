#include "Engine/3D/WorldPhysic3D/WorldPhysic3D.hpp"
#include "Engine/3D/WorldPhysic3D/DebugRenderer/DebugRenderer.hpp"
#include "Engine/3D/WorldPhysic3D/ContactListener/ContactListener.hpp"
#include "Engine/3D/WorldPhysic3D/ObjectAndBroadPhaseLayer/ObjectAndBroadPhaseLayer.hpp"
#include <Jolt/Core/Factory.h>
#include <Jolt/RegisterTypes.h>
#include <cstdarg>
#include <iostream>

static void TraceImpl(const char *inFMT, ...)
{
    va_list list;
    va_start(list, inFMT);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), inFMT, list);
    va_end(list);

    std::cout << buffer << std::endl;
}

std::unique_ptr<JPH::TempAllocatorImpl> WorldPhysic3D::tempAllocator = NULL;
std::unique_ptr<JPH::JobSystemThreadPool> WorldPhysic3D::jobSystem = NULL;
JPH::PhysicsSystem WorldPhysic3D::physicSystem;
WorldPhysic3D::BroadPhaseLayerInterface WorldPhysic3D::broadPhaseLayerInterface;
WorldPhysic3D::ObjectVsBroadPhaseLayerFilter WorldPhysic3D::objectVsBroadPhaseLayerFilter;
WorldPhysic3D::ObjectLayerPairFilter WorldPhysic3D::objectLayerPairFilter;
WorldPhysic3D::ContactListener WorldPhysic3D::contactListener;
JPH::BodyInterface &WorldPhysic3D::bodyInterface = physicSystem.GetBodyInterface();

float WorldPhysic3D::deltaTime = 1.0f / 60;
int WorldPhysic3D::collisionStep = 1;
std::map<JPH::BodyID, PhysicBody3D *> WorldPhysic3D::bodies;

void WorldPhysic3D::Init()
{
    JPH::RegisterDefaultAllocator();

    JPH::Trace = TraceImpl;
    JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;)

    JPH::Factory::sInstance = new JPH::Factory();

    JPH::RegisterTypes();

    tempAllocator = std::make_unique<JPH::TempAllocatorImpl>(10 * 1024 * 1024);
    jobSystem = std::make_unique<JPH::JobSystemThreadPool>(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, JPH::thread::hardware_concurrency() - 1);

    const uint cMaxBodies = 1024;
    const uint cNumBodyMutexes = 0;
    const uint cMaxBodyPairs = 1024;
    const uint cMaxContactConstraints = 1024;

    physicSystem.Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, broadPhaseLayerInterface, objectVsBroadPhaseLayerFilter, objectLayerPairFilter);
    physicSystem.OptimizeBroadPhase();
    physicSystem.SetContactListener(&contactListener);
}

void WorldPhysic3D::Update()
{
    for (auto it = bodies.begin(); it != bodies.end(); it++)
        it->second->OnWorldPhysicUpdated();
    physicSystem.Update(deltaTime, collisionStep, tempAllocator.get(), jobSystem.get());
}

#if DEBUG_DRAW_PHYSIC_3D
void WorldPhysic3D::DebugDraw(const JPH::BodyManager::DrawSettings &inSettings, const JPH::BodyDrawFilter *inBodyFilter)
{
    if (JPH::DebugRenderer::sInstance == nullptr)
        JPH::DebugRenderer::sInstance = new DebugRenderer();

    physicSystem.DrawBodies(inSettings, JPH::DebugRenderer::sInstance, inBodyFilter);
}
#endif

void WorldPhysic3D::Destroy()
{
    JPH::UnregisterTypes();

    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;

#if DEBUG_DRAW_PHYSIC_3D
    if (JPH::DebugRenderer::sInstance != nullptr)
    {
        delete JPH::DebugRenderer::sInstance;
        JPH::DebugRenderer::sInstance = nullptr;
    }
#endif
}

void WorldPhysic3D::AddRigidBody(RigidBody *ptr, const JPH::BodyCreationSettings &settings, JPH::EActivation inActivationMode)
{
    ptr->id = bodyInterface.CreateAndAddBody(settings, inActivationMode);
    bodies[ptr->id] = ptr;
}

void WorldPhysic3D::AddRagdoll(Ragdoll *ptr, const JPH::RagdollSettings &settings, JPH::EActivation inActivationMode)
{
    ptr->ragdoll = settings.CreateRagdoll(0, 0, &physicSystem);
    ptr->ragdoll->AddToPhysicsSystem(inActivationMode);
    for (JPH::BodyID id : ptr->ragdoll->GetBodyIDs())
        bodies[id] = ptr;
}

void WorldPhysic3D::RemoveBody(const JPH::BodyID &id)
{
    bodies.erase(id);
    bodyInterface.RemoveBody(id);
    bodyInterface.DestroyBody(id);
}