#include "Engine/UI/AComponent/AComponent.hpp"
#include "Engine/UI/ACanvas/ACanvas.hpp"
#include "Engine/macros.hpp"

namespace UI
{
AComponent::AComponent()
{
    ID = ACanvas::GenerateNewID();
    rootCanvas = ACanvas::GetCurrentCanvas();
}

void AComponent::SendEvent(int event)
{
    CHECK_AND_RETURN_VOID(rootCanvas, "rootCanvas is NULL");
    rootCanvas->HandleEvents(ID, event);
}

}; // namespace UI