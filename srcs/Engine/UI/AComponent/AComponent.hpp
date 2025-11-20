#pragma once

#include "Matrix/Matrix.hpp"
#include <vector>
#include <memory>

namespace UI
{
class ACanvas;
struct EventData;

class AComponent
{
  private:
    unsigned int ID;
    ACanvas *rootCanvas;

  protected:
    ml::vec2 pos;

    void SendEvent(EventData &data);

  public:
    AComponent();
    virtual ~AComponent() {}

    virtual void Update() {}
    virtual void Draw() {}

    unsigned int GetID() { return ID; }
};
} // namespace UI