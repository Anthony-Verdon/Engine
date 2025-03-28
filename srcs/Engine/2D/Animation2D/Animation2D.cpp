#include "Engine/2D/Animation2D/Animation2D.hpp"
#include "Engine/Time/Time.hpp"
#include <iostream>

Animation2D Animation2D::none;

Animation2D::Animation2D(float animationSpeed, bool stoppable)
{
    this->animationSpeed = animationSpeed;
    this->stoppable = stoppable;
    timer = 0;
    frameIndex = 0;
    ended = false;
}

Animation2D::~Animation2D()
{
    
}

void Animation2D::AddFrame(const Sprite &frame)
{
    frames.push_back(frame);
}

void Animation2D::SetFrames(const std::vector<Sprite> &frames)
{
    this->frames = frames;
}

void Animation2D::SetAnimationSpeed(float animationSpeed)
{
    this->animationSpeed = animationSpeed;
}

void Animation2D::SetStoppable(bool stoppable)
{
    this->stoppable = stoppable;
}

void Animation2D::Update()
{
    if (frames.size() == 0)
        return;

    timer += Time::getDeltaTime();
    if (timer > animationSpeed)
    {
        timer = 0;
        frameIndex++;
        if (frameIndex == frames.size())
        {
            frameIndex = 0;
            ended = true;
        }
    }
}

void Animation2D::Reset()
{
    timer = 0;
    frameIndex = 0;
    ended = false;
}

Sprite Animation2D::GetFrame() const
{
    if (frames.size() == 0)
        std::cerr << "Animation: no frame" << std::endl; //@todo: create a default Sprite value (like Animation::none) with an error texture and return it

    return (frames[frameIndex]);
}

std::vector<Sprite> Animation2D::GetFrames() const
{
    return (frames);
}

bool Animation2D::IsStoppable() const
{
    return (stoppable);
}

bool Animation2D::Ended() const
{
    return (ended);
}

unsigned int Animation2D::GetFrameIndex() const
{
    return (frameIndex);
}