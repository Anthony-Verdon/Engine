#pragma once

#include <memory>

class AScene
{
  public:
    AScene() {}
    virtual ~AScene() {}

    virtual void Load() {}
    virtual std::unique_ptr<AScene> Run() { return NULL; }
    virtual void Quit() {}

    virtual int GetID() = 0;
};