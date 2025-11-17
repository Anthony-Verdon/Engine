#pragma once

#include <memory>

class AScene
{
  public:
    AScene() {}
    virtual ~AScene() {}

    virtual void Load() {}
    virtual void Run() {}
    virtual void Quit() {}

    virtual int GetID() = 0;
};