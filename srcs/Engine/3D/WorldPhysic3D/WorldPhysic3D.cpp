#include "Engine/3D/WorldPhysic3D/WorldPhysic3D.hpp"
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
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
JPH::BodyInterface &WorldPhysic3D::bodyInterface = physicSystem.GetBodyInterface();

void WorldPhysic3D::Init(const JPH::BroadPhaseLayerInterface &BPLayerInterface, const JPH::ObjectVsBroadPhaseLayerFilter &objVsBPLayerFilter, const JPH::ObjectLayerPairFilter &OLPFilter)
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

	physicSystem.Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, BPLayerInterface, objVsBPLayerFilter, OLPFilter);
    physicSystem.OptimizeBroadPhase();
}

void WorldPhysic3D::Update()
{
    const float cDeltaTime = 1.0f / 60.0f;
    const int cCollisionSteps = 1;
    physicSystem.Update(cDeltaTime, cCollisionSteps, tempAllocator.get(), jobSystem.get());
}

void WorldPhysic3D::Destroy()
{
    JPH::UnregisterTypes();

    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}