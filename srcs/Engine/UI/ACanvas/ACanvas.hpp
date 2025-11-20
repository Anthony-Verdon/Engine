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
    static ACanvas *currentCanvas;
    static unsigned int currentID;

  public:
    ACanvas() {}
    virtual ~ACanvas() {}

    void Update();
    std::unique_ptr<AComponent> &GetComponent(unsigned int ID);
    virtual void HandleEvents([[maybe_unused]] EventData &data);

    static void BeginCanvas(ACanvas *canvasPtr);
    static unsigned int AddComponent(std::unique_ptr<AComponent> component);
    static void EndCanvas();

    static ACanvas *GetCurrentCanvas();
    static unsigned int GenerateNewID();
};
}; // namespace UI