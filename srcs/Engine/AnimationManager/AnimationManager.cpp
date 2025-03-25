#include "Engine/AnimationManager/AnimationManager.hpp"
#include "Engine/macros.hpp"

AnimationManager::AnimationManager()
{
    currentAnimation = "none";
    animations[currentAnimation] = Animation::none;
}

AnimationManager::~AnimationManager()
{

}

void AnimationManager::AddAnimation(const std::string &name, const Animation &animation)
{
    animations[name] = animation;
}

void AnimationManager::Play(const std::string &name)
{
    if (!animations[currentAnimation].IsStoppable() && !animations[currentAnimation].Ended())
        return;
    
    CHECK_AND_RETURN_VOID((animations.find(name) != animations.end()), "animation " + name + " not found");

    if (currentAnimation == name)
        return;

    currentAnimation = name;
    animations[currentAnimation].Reset();
}

void AnimationManager::Update()
{
    if (currentAnimation != "none")
        animations[currentAnimation].Update();
}

Sprite AnimationManager::GetFrame() const
{
    auto it = animations.find(currentAnimation);
    if (it == animations.end())
        std::cerr << "AnimationManager::GetFrame() : no animation added" << std::endl; //@todo: create a default Sprite value (like Animation::none) with an error texture and return it
    return (it->second.GetFrame());
}

bool AnimationManager::CurrentAnimationEnded() const
{
    auto it = animations.find(currentAnimation);
    if (it == animations.end())
        std::cerr << "AnimationManager::CurrentAnimationEnded() : no animation added" << std::endl; //@todo: create a default Sprite value (like Animation::none) with an error texture and return it
    return (it->second.IsStoppable() || it->second.Ended());
}

std::string AnimationManager::GetCurrentAnimation() const
{
    return (currentAnimation);
}