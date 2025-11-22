#include "Engine/UI/AComponent/AComponent.hpp"
#include "Engine/UI/ACanvas/ACanvas.hpp"
#include "Engine/macros.hpp"
#include "Engine/UI/Events.hpp"

namespace UI
{
AComponent::AComponent()
{
    show = true;
}

void AComponent::SendEvent(EventData &data)
{
    CHECK_AND_RETURN_VOID(rootCanvas, "rootCanvas is NULL");
    data.componentID = ID;
    rootCanvas->HandleEvents(data);
}

}; // namespace UI