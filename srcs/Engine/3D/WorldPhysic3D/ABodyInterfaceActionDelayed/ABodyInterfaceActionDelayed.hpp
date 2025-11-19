#pragma once

#include <Jolt/Physics/Body/BodyID.h>

enum class BodyInterfaceActionType
{
    DEACTIVATE_BODY,
    REMOVE_BODY
};

struct ABodyInterfaceActionDelayed
{
    BodyInterfaceActionType type;
    JPH::BodyID id;

    virtual ~ABodyInterfaceActionDelayed() {}
    BodyInterfaceActionType GetType() { return type; }
};

struct DeactivateBodyAction : public ABodyInterfaceActionDelayed
{
    DeactivateBodyAction()
    {
        type = BodyInterfaceActionType::DEACTIVATE_BODY;
    }
};

struct RemoveBodyAction : public ABodyInterfaceActionDelayed
{
    RemoveBodyAction()
    {
        type = BodyInterfaceActionType::REMOVE_BODY;
    }
};