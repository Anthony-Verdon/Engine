#pragma once

#include <box2d/box2d.h>
#include <glm/glm.hpp>
#include <string>
#include <map>
#include <memory>

class PhysicBody
{
    private:
        std::shared_ptr<b2BodyId> id;
        std::map<std::string, b2ShapeId> shapes;

    public:
        PhysicBody();
        PhysicBody(const b2WorldId& worldId, const b2BodyDef& bodyDef);
        ~PhysicBody();

        void AddShape(const std::string &name, const b2ShapeDef& shapeDef, const b2Polygon& polygon);

        b2BodyId GetBodyId() const;
        b2ShapeId GetShape(const std::string &name) const;
        glm::vec2 GetPosition() const;
        float GetAngle() const;


        static float WorldToPixel(float value);
        static float PixelToWorld(float value);

        class BodyBuilder
        {
            private:
                b2BodyDef bodyDef;
            
            public:
                BodyBuilder();
                ~BodyBuilder();

                BodyBuilder& SetPosition(const glm::vec2 &position);
                BodyBuilder& SetEnable(bool enable);
                BodyBuilder& SetType(b2BodyType type);
                BodyBuilder& SetFixedRotation(bool fixedRotation);

                PhysicBody Build(const b2WorldId &worldId);
        };

        class ShapeBuilder
        {
            private:
                b2ShapeDef shapeDef;
            
            public:
                ShapeBuilder();
                ~ShapeBuilder();

                ShapeBuilder& SetDensity(float density);
                ShapeBuilder& SetFriction(float friction);
                ShapeBuilder& SetFilter(const b2Filter &filter);
                ShapeBuilder& IsSensor(bool isSensor);
                ShapeBuilder& SetUserData(void *ptr);

                b2ShapeDef Build();
        };

        class PolygonBuilder
        {
            public:
                PolygonBuilder() = delete;
                ~PolygonBuilder() = delete;

                static b2Polygon Build(const glm::vec2 &size, const glm::vec2 &position = glm::vec2(0, 0), float rotation = 0);
        };
};