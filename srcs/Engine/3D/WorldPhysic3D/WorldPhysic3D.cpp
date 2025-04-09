#include "Engine/3D/WorldPhysic3D/WorldPhysic3D.hpp"

static void TraceImpl(const char *inFMT, ...)
{
	va_list list;
	va_start(list, inFMT);
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), inFMT, list);
	va_end(list);

	std::cout << buffer << std::endl;
}

BPLayerInterfaceImpl WorldPhysic3D::broad_phase_layer_interface;
ObjectVsBroadPhaseLayerFilterImpl WorldPhysic3D::object_vs_broadphase_layer_filter;
ObjectLayerPairFilterImpl WorldPhysic3D::object_vs_object_layer_filter;

MyBodyActivationListener WorldPhysic3D::body_activation_listener;
MyContactListener WorldPhysic3D::contact_listener;

std::unique_ptr<JPH::TempAllocatorImpl> WorldPhysic3D::temp_allocator = NULL;
std::unique_ptr<JPH::JobSystemThreadPool> WorldPhysic3D::job_system = NULL;
JPH::PhysicsSystem WorldPhysic3D::physics_system;
JPH::BodyInterface &WorldPhysic3D::body_interface = physics_system.GetBodyInterface();

JPH::Body *WorldPhysic3D::floor;
JPH::BodyID WorldPhysic3D::sphere_id;

void WorldPhysic3D::Init()
{
    JPH::RegisterDefaultAllocator();

	JPH::Trace = TraceImpl;
	JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;)

	JPH::Factory::sInstance = new JPH::Factory();

    JPH::RegisterTypes();

    temp_allocator = std::make_unique<JPH::TempAllocatorImpl>(10 * 1024 * 1024);
    job_system = std::make_unique<JPH::JobSystemThreadPool>(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, JPH::thread::hardware_concurrency() - 1);
	
    const uint cMaxBodies = 1024;
	const uint cNumBodyMutexes = 0;
	const uint cMaxBodyPairs = 1024;
	const uint cMaxContactConstraints = 1024;

	physics_system.Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter);

	physics_system.SetBodyActivationListener(&body_activation_listener);
	physics_system.SetContactListener(&contact_listener);

    physics_system.OptimizeBroadPhase();

    // -----------------------------------
	JPH::BoxShapeSettings floor_shape_settings(JPH::Vec3(100.0f, 1.0f, 100.0f));
	floor_shape_settings.SetEmbedded();
    
	JPH::ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
	JPH::ShapeRefC floor_shape = floor_shape_result.Get();
    
	JPH::BodyCreationSettings floor_settings(floor_shape, JPH::RVec3(0.0, -1.0, 0.0), JPH::Quat::sIdentity(), JPH::EMotionType::Static, Layers::NON_MOVING);
	floor = body_interface.CreateBody(floor_settings);
	body_interface.AddBody(floor->GetID(), JPH::EActivation::DontActivate);
    
	JPH::BodyCreationSettings sphere_settings(new JPH::SphereShape(0.5f), JPH::RVec3(0.0, 2.0, 0.0), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Layers::MOVING);
	sphere_id = body_interface.CreateAndAddBody(sphere_settings, JPH::EActivation::Activate);
	body_interface.SetLinearVelocity(sphere_id, JPH::Vec3(0.0f, -5.0f, 0.0f));
}

void WorldPhysic3D::Update()
{
    static uint step = 0;

    ++step;

    if (body_interface.IsActive(sphere_id))
    {
        JPH::RVec3 position = body_interface.GetCenterOfMassPosition(sphere_id);
        JPH::Vec3 velocity = body_interface.GetLinearVelocity(sphere_id);
        std::cout << "Step " << step << ": Position = (" << position.GetX() << ", " << position.GetY() << ", " << position.GetZ() << "), Velocity = (" << velocity.GetX() << ", " << velocity.GetY() << ", " << velocity.GetZ() << ")" << std::endl;
    }
    
    const float cDeltaTime = 1.0f / 60.0f;
    const int cCollisionSteps = 1;
    physics_system.Update(cDeltaTime, cCollisionSteps, temp_allocator.get(), job_system.get());
}

void WorldPhysic3D::Destroy()
{
    JPH::UnregisterTypes();

    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}