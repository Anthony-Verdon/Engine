#pragma once

#include <box2d/box2d.h>

class WorldPhysic
{
  private:
    WorldPhysic() = delete;
    ~WorldPhysic() = delete;

    static b2WorldId worldId;
    static float timeStep;
    static int subStepCount;
    static b2DebugDraw debugDraw;

    static void InitDebugDraw();

  public:
    static void Init();
    static void Destroy() { b2DestroyWorld(worldId); }

    static void Update();
    static void DebugDraw() { b2World_Draw(worldId, &debugDraw); }

    static const b2WorldId GetWorldId() { return worldId; }
};