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
    bool show;

  protected:
    ml::vec2 pos;

    void SendEvent(EventData &data);

  public:
    AComponent();
    AComponent(const ml::vec2 &pos);
    virtual ~AComponent() {}

    virtual void Update() {}
    virtual void Draw() {}

    void Show() { show = true; }
    void Hide() { show = false; }

    friend ACanvas;
};
} // namespace UI