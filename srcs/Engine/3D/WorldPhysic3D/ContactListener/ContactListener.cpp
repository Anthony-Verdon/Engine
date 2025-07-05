#include "Engine/3D/WorldPhysic3D/ContactListener/ContactListener.hpp"
#include <Jolt/Physics/Body/Body.h>

JPH::ValidateResult WorldPhysic3D::ContactListener::OnContactValidate(const JPH::Body &inBody1, const JPH::Body &inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult &inCollisionResult)
{
    (void)inBody1;
    (void)inBody2;
    (void)inBaseOffset;
    (void)inCollisionResult;

    return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

void WorldPhysic3D::ContactListener::OnContactAdded(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings)
{
    (void)ioSettings;

    {
        Contact newContact;
        newContact.id = inBody2.GetID();
        newContact.normal = inManifold.mWorldSpaceNormal;
        contacts[inBody1.GetID()].push_back(newContact);
    }
    {
        Contact newContact;
        newContact.id = inBody1.GetID();
        newContact.normal = -inManifold.mWorldSpaceNormal;
        contacts[inBody2.GetID()].push_back(newContact);
    }
}

void WorldPhysic3D::ContactListener::OnContactPersisted(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings)
{
    (void)inBody1;
    (void)inBody2;
    (void)inManifold;
    (void)ioSettings;
}

void WorldPhysic3D::ContactListener::OnContactRemoved(const JPH::SubShapeIDPair &inSubShapePair)
{
    {
        auto contactMapIt = contacts.find(inSubShapePair.GetBody1ID());
        if (contactMapIt != contacts.end())
        {
            for (auto contactVectorIt = contactMapIt->second.begin();
                 contactVectorIt != contactMapIt->second.end();
                 contactVectorIt++)
            {
                if (contactVectorIt->id == inSubShapePair.GetBody2ID())
                {
                    contactMapIt->second.erase(contactVectorIt);
                    break;
                }
            }
        }
    }
    {
        auto contactMapIt = contacts.find(inSubShapePair.GetBody2ID());
        if (contactMapIt != contacts.end())
        {
            for (auto contactVectorIt = contactMapIt->second.begin();
                 contactVectorIt != contactMapIt->second.end();
                 contactVectorIt++)
            {
                if (contactVectorIt->id == inSubShapePair.GetBody1ID())
                {
                    contactMapIt->second.erase(contactVectorIt);
                    break;
                }
            }
        }
    }
}

std::vector<Contact> WorldPhysic3D::ContactListener::GetContacts(const JPH::BodyID &id)
{
    auto contactMapIt = contacts.find(id);
    if (contactMapIt != contacts.end())
        return (contactMapIt->second);
    else
        return (std::vector<Contact>{});
}