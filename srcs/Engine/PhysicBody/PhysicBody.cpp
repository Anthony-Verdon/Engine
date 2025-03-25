#include "Engine/PhysicBody/PhysicBody.hpp"
#include <iostream>
#include "Engine/defines.hpp"

//***************************************
// PHYSICBODY
//***************************************

PhysicBody::PhysicBody()
{
    id = std::make_shared<b2BodyId>(b2_nullBodyId);
}

PhysicBody::PhysicBody(const b2WorldId& worldId, const b2BodyDef& bodyDef)
{
    id = std::make_shared<b2BodyId>(b2CreateBody(worldId, &bodyDef));
}


PhysicBody::~PhysicBody()
{
    if (id.use_count() == 1 && b2Body_IsValid(*id))
        b2DestroyBody(*id);
}

void PhysicBody::AddShape(const std::string &name, const b2ShapeDef& shapeDef, const b2Polygon& polygon)
{
    shapes[name] = b2CreatePolygonShape(*id, &shapeDef, &polygon);
}

float PhysicBody::WorldToPixel(float value)
{
    return (value * PIXEL_PER_METER);
}

float PhysicBody::PixelToWorld(float value)
{
    return (value / PIXEL_PER_METER);
}

b2BodyId PhysicBody::GetBodyId() const
{
    return (*id);
}

b2ShapeId PhysicBody::GetShape(const std::string &name) const
{
    auto it = shapes.find(name);
    if (it != shapes.end())
        return (it->second);
    else
        return (b2_nullShapeId);
}

glm::vec2 PhysicBody::GetPosition() const
{
    b2Vec2 worldPosition = b2Body_GetPosition(*id);
    return (glm::vec2(WorldToPixel(worldPosition.x), WorldToPixel(worldPosition.y)));
}

float PhysicBody::GetAngle() const
{
    return (glm::degrees(b2Rot_GetAngle(b2Body_GetRotation(*id))));
}

//***************************************
// BODYBUILDER
//***************************************

PhysicBody::BodyBuilder::BodyBuilder()
{
    bodyDef = b2DefaultBodyDef();
}

PhysicBody::BodyBuilder::~BodyBuilder()
{

}

PhysicBody::BodyBuilder& PhysicBody::BodyBuilder::SetPosition(const glm::vec2 &position)
{
    bodyDef.position = (b2Vec2){PixelToWorld(position.x), PixelToWorld(position.y)};
    return (*this);
}

PhysicBody::BodyBuilder& PhysicBody::BodyBuilder::SetEnable(bool enable)
{
    bodyDef.isEnabled = enable;
    return (*this);
}

PhysicBody::BodyBuilder& PhysicBody::BodyBuilder::SetType(b2BodyType type)
{
    bodyDef.type = type;
    return (*this);
}

PhysicBody::BodyBuilder& PhysicBody::BodyBuilder::SetFixedRotation(bool fixedRotation)
{
    bodyDef.fixedRotation = fixedRotation;
    return (*this);
}

PhysicBody PhysicBody::BodyBuilder::Build(const b2WorldId &worldId)
{
    return (PhysicBody(worldId, bodyDef));
}

//***************************************
// SHAPEBUILDER
//***************************************
PhysicBody::ShapeBuilder::ShapeBuilder()
{
    shapeDef = b2DefaultShapeDef();
}

PhysicBody::ShapeBuilder::~ShapeBuilder()
{
}

PhysicBody::ShapeBuilder& PhysicBody::ShapeBuilder::SetDensity(float density)
{
    shapeDef.density = density;
    return (*this);
}

PhysicBody::ShapeBuilder& PhysicBody::ShapeBuilder::SetFriction(float friction)
{
    shapeDef.friction = friction;
    return (*this);
}

PhysicBody::ShapeBuilder& PhysicBody::ShapeBuilder::SetFilter(const b2Filter &filter)
{
    shapeDef.filter = filter;
    return (*this);
}

PhysicBody::ShapeBuilder& PhysicBody::ShapeBuilder::IsSensor(bool isSensor)
{
    shapeDef.isSensor = isSensor;
    return (*this);
}

PhysicBody::ShapeBuilder& PhysicBody::ShapeBuilder::SetUserData(void *ptr)
{
    shapeDef.userData = ptr;
    return (*this);
}

b2ShapeDef PhysicBody::ShapeBuilder::Build()
{
    return (shapeDef);
}

//***************************************
// POLYGONBUILDER
//***************************************

b2Polygon PhysicBody::PolygonBuilder::Build(const glm::vec2 &size, const glm::vec2 &position, float rotation)
{
    if (position == glm::vec2(0, 0))
    {
        return (b2MakeBox(PixelToWorld(size.x / 2), PixelToWorld(size.y / 2)));
    }
    else
    {
        b2Vec2 points[4];
        points[0] = {PixelToWorld(-size.x / 2), PixelToWorld(-size.y / 2)};
        points[1] = {PixelToWorld(size.x / 2), PixelToWorld(-size.y / 2)};
        points[2] = {PixelToWorld(size.x / 2), PixelToWorld(size.y / 2)};
        points[3] = {PixelToWorld(-size.x / 2), PixelToWorld(size.y / 2)};
        b2Hull hull = b2ComputeHull(points, 4);

        b2Transform transform;
        transform.p = (b2Vec2){PixelToWorld(position.x), PixelToWorld(position.y)};
        transform.q = b2MakeRot(glm::radians(rotation));

        return (b2MakeOffsetPolygon(&hull, 0, transform));
    }
}