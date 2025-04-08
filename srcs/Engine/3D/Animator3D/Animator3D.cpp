#include "Engine/3D/Animator3D/Animator3D.hpp"

Animator3D::Animator3D()
{
    currentAnimation = "";
}

Animator3D::~Animator3D()
{
}

void Animator3D::LoadAnimation(const std::vector<Glb::Node> &nodes, const Glb::Animation &animation)
{
    animations.insert(std::make_pair(animation.name, Animation3D(nodes, animation)));
}

void Animator3D::Update()
{
    auto animation = animations.find(currentAnimation);
    if (animation != animations.end())
        animation->second.Update();
}

ml::mat4 Animator3D::GetNodeTransform(size_t node) const
{
    auto animation = animations.find(currentAnimation);
    if (animation != animations.end())
    {
        return (animation->second.GetNodeTransform(node));
    }
    else
    {
        ml::mat4 matrix;
        matrix.identity();
        return (matrix);
    }
}

std::vector<std::string> Animator3D::GetAnimationsName() const
{
    std::vector<std::string> animationsName;

    for (auto it = animations.begin(); it != animations.end(); it++)
        animationsName.push_back(it->first);

    return (animationsName);
}

void Animator3D::Play(const std::string &name)
{
    auto animation = animations.find(name);
    if (animation != animations.end() && currentAnimation != name)
    {
        currentAnimation = name;
        animation->second.Reset();
    }
}

bool Animator3D::CurrentAnimationEnded() const
{
    auto animation = animations.find(currentAnimation);
    if (animation != animations.end())
        return (animation->second.Ended());
    return (true);
}

float Animator3D::CurrentAnimationDuration() const
{
    auto animation = animations.find(currentAnimation);
    if (animation != animations.end())
        return (animation->second.AnimationDuration());
    return (1.0f);
}