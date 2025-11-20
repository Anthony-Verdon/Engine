#include "Engine/UI/ACanvas/ACanvas.hpp"
#include "Engine/macros.hpp"
#include "Engine/UI/Events.hpp"

namespace UI
{
ACanvas *ACanvas::currentCanvas = NULL;
unsigned int ACanvas::currentID = 0;

void ACanvas::Update()
{
    for (const auto &component : components)
    {
        component->Draw();
    }

    for (const auto &component : components)
    {
        component->Update();
    }
}

std::unique_ptr<AComponent> &ACanvas::GetComponent(unsigned int ID)
{
    for (auto &component : components)
    {
        if (component->GetID() == ID)
            return (component);
    }

    throw(std::runtime_error("component not found"));
}

void ACanvas::HandleEvents([[maybe_unused]] EventData &data)
{
}

void ACanvas::BeginCanvas(ACanvas *canvasPtr)
{
    CHECK_AND_RETURN_VOID((currentCanvas == NULL), "a canvas already exists");
    currentCanvas = canvasPtr;
    currentID = 0;
}

unsigned int ACanvas::AddComponent(std::unique_ptr<AComponent> component)
{
    CHECK_AND_RETURN((currentCanvas != NULL), 0, "no current canvas");
    currentCanvas->components.push_back(std::move(component));
    return (currentCanvas->components[currentCanvas->components.size() - 1]->GetID());
}

void ACanvas::EndCanvas()
{
    CHECK_AND_RETURN_VOID((currentCanvas != NULL), "no current canvas");
    currentCanvas = NULL;
}

ACanvas *ACanvas::GetCurrentCanvas()
{
    CHECK_AND_RETURN((currentCanvas != NULL), NULL, "no current canvas");
    return currentCanvas;
}

unsigned int ACanvas::GenerateNewID()
{
    CHECK_AND_RETURN((currentCanvas != NULL), 0, "no current canvas");
    return ++currentID;
}

} // namespace UI