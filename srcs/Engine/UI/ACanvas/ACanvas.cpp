#include "Engine/UI/ACanvas/ACanvas.hpp"
#include "Engine/macros.hpp"
#include "Engine/UI/Events.hpp"
#include <glad/glad.h>

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
    // I'm developping a 3D game so GL_DEPTH_TEST is enable by default in WindowManager
    // for 2D/UI rendering, we need to disable it, and I re-enable it after
    // but if I develop a 2D game, maybe we should have a parameter in WindowManager to tell us
    // if is GL_DEPTH_TEST should be enable or not
    glDisable(GL_DEPTH_TEST);
    for (const auto &component : components)
    {
        if (component->show)
            component->Draw();
    }

    for (const auto &component : components)
    {
        if (component->show)
            component->Update();
    }
    glEnable(GL_DEPTH_TEST);
}

void ACanvas::HandleEvents([[maybe_unused]] EventData &data)
{
}

} // namespace UI