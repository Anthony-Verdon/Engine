#pragma once

#include "Engine/UI/AComponent/AComponent.hpp"
#include <vector>
#include <memory>

namespace UI
{

struct EventData;

class ACanvas
{
  private:
    std::vector<std::unique_ptr<AComponent>> components;
    ComponentID currentID;

  protected:
    ComponentID AddComponent(std::unique_ptr<AComponent> component);
    std::unique_ptr<AComponent> &GetComponent(ComponentID ID);
    void DestroyComponent(ComponentID ID);
    void ResetCanvas();

  public:
    ACanvas();
    virtual ~ACanvas() {}

    void Update();
    virtual void HandleEvents([[maybe_unused]] EventData &data);
};
}; // namespace UI