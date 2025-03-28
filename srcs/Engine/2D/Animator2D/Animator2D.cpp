#include "Engine/2D/Animator2D/Animator2D.hpp"
#include "Engine/macros.hpp"

Animator2D::Animator2D()
{
    currentAnimation = "none";
    animations[currentAnimation] = Animation2D::none;
}

Animator2D::~Animator2D()
{

}

void Animator2D::AddAnimation(const std::string &name, const Animation2D &animation)
{
    animations[name] = animation;
}

void Animator2D::Play(const std::string &name)
{
    if (!animations[currentAnimation].IsStoppable() && !animations[currentAnimation].Ended())
        return;
    
    CHECK_AND_RETURN_VOID((animations.find(name) != animations.end()), "animation " + name + " not found");

    if (currentAnimation == name)
        return;

    currentAnimation = name;
    animations[currentAnimation].Reset();
}

void Animator2D::Update()
{
    if (currentAnimation != "none")
        animations[currentAnimation].Update();
}

Sprite Animator2D::GetFrame() const
{
    auto it = animations.find(currentAnimation);
    if (it == animations.end())
        std::cerr << "AnimationManager::GetFrame() : no animation added" << std::endl; //@todo: create a default Sprite value (like Animation::none) with an error texture and return it
    return (it->second.GetFrame());
}

bool Animator2D::CurrentAnimationEnded() const
{
    auto it = animations.find(currentAnimation);
    if (it == animations.end())
        std::cerr << "AnimationManager::CurrentAnimationEnded() : no animation added" << std::endl; //@todo: create a default Sprite value (like Animation::none) with an error texture and return it
    return (it->second.IsStoppable() || it->second.Ended());
}

std::string Animator2D::GetCurrentAnimation() const
{
    return (currentAnimation);
}