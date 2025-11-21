#include "Engine/UI/ACanvas/ACanvas.hpp"
#include "Engine/macros.hpp"
#include "Engine/UI/Events.hpp"

namespace UI
{

ACanvas::ACanvas()
{
    ResetCanvas();
}

ComponentID ACanvas::AddComponent(std::unique_ptr<AComponent> component)
{
    component->ID = currentID++;
    component->rootCanvas = this;
    components.push_back(std::move(component));
    return (components[components.size() - 1]->ID);
}

std::unique_ptr<AComponent> &ACanvas::GetComponent(ComponentID ID)
{
    for (auto &component : components)
    {
        if (component->ID == ID)
            return (component);
    }

    throw(std::runtime_error("component not found"));
}

void ACanvas::DestroyComponent(ComponentID ID)
{
    for (size_t i = 0; i < components.size(); i++)
    {
        if (components[i]->ID == ID)
        {
            components.erase(components.begin() + i);
            break;
        }
    }
}

void ACanvas::ResetCanvas()
{
    currentID = 0;
    components.clear();
}

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

void ACanvas::HandleEvents([[maybe_unused]] EventData &data)
{
}

} // namespace UI