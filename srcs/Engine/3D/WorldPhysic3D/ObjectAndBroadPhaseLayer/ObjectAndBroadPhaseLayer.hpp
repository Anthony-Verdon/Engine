#pragma once

#include <Jolt/Core/Factory.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/RegisterTypes.h>
#include <map>
#include <vector>

class WorldPhysic3D::BroadPhaseLayerInterface final : public JPH::BroadPhaseLayerInterface
{
  public:
#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    void AddObjectToBroadPhaseLayerRelation(JPH::ObjectLayer objectLayer, JPH::BroadPhaseLayer broadPhaseLayer, const std::string &BroadPhaseLayerName)
    {
        objectToBroadPhase[objectLayer] = broadPhaseLayer;
        broadPhaseNames[broadPhaseLayer] = BroadPhaseLayerName;
    }
#else
    void AddObjectToBroadPhaseLayerRelation(JPH::ObjectLayer objectLayer, JPH::BroadPhaseLayer broadPhaseLayer)
    {
        objectToBroadPhase[objectLayer] = broadPhaseLayer;
    }
#endif
    virtual uint GetNumBroadPhaseLayers() const override
    {
        return objectToBroadPhase.size();
    }

    virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
    {
        JPH_ASSERT(inLayer < objectToBroadPhase.size());
        return (objectToBroadPhase.find(inLayer)->second);
    }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    virtual const char *GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override
    {
        return (broadPhaseNames.find(inLayer)->second.c_str());
    }
#endif

  private:
#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    std::map<JPH::BroadPhaseLayer, std::string> broadPhaseNames;
#endif
    std::map<JPH::ObjectLayer, JPH::BroadPhaseLayer> objectToBroadPhase;
};

class WorldPhysic3D::ObjectVsBroadPhaseLayerFilter : public JPH::ObjectVsBroadPhaseLayerFilter
{
  public:
    void AddObjectToBroadPhaseLayerRelation(JPH::ObjectLayer objectLayer, JPH::BroadPhaseLayer broadPhaseLayer)
    {
        objectToBroadPhase[objectLayer].push_back(broadPhaseLayer);
    }

    virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
    {
        auto broadPhaseLayers = objectToBroadPhase.find(inLayer1)->second;
        return (std::find(broadPhaseLayers.begin(), broadPhaseLayers.end(), inLayer2) != broadPhaseLayers.end());
    }

  private:
    std::map<JPH::ObjectLayer, std::vector<JPH::BroadPhaseLayer>> objectToBroadPhase;
};

class WorldPhysic3D::ObjectLayerPairFilter : public JPH::ObjectLayerPairFilter
{
  public:
    void AddObjectToObjectRelation(JPH::ObjectLayer objectLayer1, JPH::ObjectLayer objectLayer2)
    {
        objectToObject[objectLayer1].push_back(objectLayer2);
    }

    virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override
    {
        auto objectLayers = objectToObject.find(inObject1)->second;
        return (std::find(objectLayers.begin(), objectLayers.end(), inObject2) != objectLayers.end());
    }

  private:
    std::map<JPH::ObjectLayer, std::vector<JPH::ObjectLayer>> objectToObject;
};
