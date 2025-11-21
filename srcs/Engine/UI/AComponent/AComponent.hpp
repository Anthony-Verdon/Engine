#pragma once

#include "Matrix/Matrix.hpp"
#include <vector>
#include <memory>

namespace UI
{
class ACanvas;
struct EventData;
typedef unsigned int ComponentID;

class AComponent
{
  private:
    ComponentID ID;
    ACanvas *rootCanvas;

  protected:
    ml::vec2 pos;

    void SendEvent(EventData &data);

  public:
    AComponent();
    virtual ~AComponent() {}

    virtual void Update() {}
    virtual void Draw() {}

    friend ACanvas;
};
} // namespace UI