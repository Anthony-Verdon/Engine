#pragma once

#include <vector>
#include "Engine/UI/AComponent/AComponent.hpp"
#include "Engine/2D/Sprite/Sprite.hpp"

namespace UI
{
template <typename T>
class Slider : public AComponent
{
  private:
    Slider() = delete;

    T min;
    T max;
    T value; // use reference ?

    ml::vec2 size;
    Sprite boundarySprite;
    ml::vec2 leftBoundaryPos, rightBoundaryPos;
    Sprite pipeSprite;
    Sprite thumbSprite;
    ml::vec2 thumbPos;

    bool clicked;

  public:
    Slider(const Sprite &boundarySprite, const Sprite &pipeSprite, const Sprite &thumbSprite, T min, T max, T defaultValue, const ml::vec2 &pos, const ml::vec2 &size);
    ~Slider();

    void Update();
    void Draw();

    struct UpdateValueEventData : public EventData
    {
        T newValue;

        UpdateValueEventData(T newValue) : EventData(UPDATE_VALUE), newValue(newValue) {}
    };
};

typedef Slider<float> FloatSlider;
typedef Slider<int> IntSlider;

}; // namespace UI

#include "Engine/UI/Slider/Slider.tpp"