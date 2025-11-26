#include "Engine/UI/AComponent/AComponent.hpp"
#include "Engine/UI/ACanvas/ACanvas.hpp"
#include "Engine/macros.hpp"
#include "Engine/UI/Callbacks.hpp"

namespace UI
{
AComponent::AComponent()
{
    show = true;
}

AComponent::AComponent(const ml::vec2 &pos) : pos(pos)
{
    show = true;
}

}; // namespace UI