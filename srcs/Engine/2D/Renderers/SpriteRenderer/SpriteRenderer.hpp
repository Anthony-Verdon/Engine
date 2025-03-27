#pragma once

#include "Matrix/Matrix.hpp"

#include <string>
#include "Engine/2D/Sprite/Sprite.hpp"

struct SpriteRenderData
{
    ml::vec2 position;
    ml::vec2 size;
    float rotation;
    ml::vec3 color;
    Sprite sprite;
    bool flipHorizontally;
    bool flipVertically;
    float opacity;
    bool drawAbsolute;

    SpriteRenderData()
    {
        position = ml::vec2(0, 0);
        size = ml::vec2(0, 0);
        rotation = 0;
        color = ml::vec3(1, 1, 1);
        sprite = Sprite::none;
        flipHorizontally = false;
        flipVertically = false;
        opacity = 1;
        drawAbsolute = false;
    }
};

class SpriteRenderDataBuilder
{
    private:
        SpriteRenderData data;
    
    public:
        SpriteRenderDataBuilder() {}
        ~SpriteRenderDataBuilder() {}

        SpriteRenderDataBuilder& SetPosition(const ml::vec2 &position) { data.position = position; return *this; }
        SpriteRenderDataBuilder& SetSize(const ml::vec2 &size) { data.size = size; return *this; }
        SpriteRenderDataBuilder& SetRotation(float rotation) { data.rotation = rotation; return *this; }
        SpriteRenderDataBuilder& SetColor(const ml::vec3 &color) { data.color = color; return *this; }
        SpriteRenderDataBuilder& SetSprite(const Sprite &sprite) { data.sprite = sprite; return *this; }
        SpriteRenderDataBuilder& FlipHorizontally( bool flip) { data.flipHorizontally = flip; return *this; }
        SpriteRenderDataBuilder& FlipVertically(bool flip) { data.flipVertically = flip; return *this; }
        SpriteRenderDataBuilder& SetOpacity(float opacity) { data.opacity = opacity; return *this; }
        SpriteRenderDataBuilder& SetDrawAbsolute(bool drawAbsolute) { data.drawAbsolute = drawAbsolute; return *this; }

        SpriteRenderData Build() {return data; }
};

class SpriteRenderer
{
    private:
        static unsigned int VAO;
        static unsigned int VBO;
        static bool isInit;

        static ml::mat4 projectionMatAbsolute;
        static ml::mat4 projectionMatRelative;
        
        SpriteRenderer() = delete;
        ~SpriteRenderer() = delete;
        
    public: 
        static void Init();
        static void Destroy();
        static void Draw(const SpriteRenderData &data);
        static void Draw(const ml::vec2 &position, const ml::vec2 &size, float rotation, const ml::vec3 &color, const Sprite &sprite, bool flipHorizontally, bool flipVertically, float opacity, bool drawAbsolute = false);

        static void SetProjectionMatRelative(const ml::mat4 &projectionMatRelative) { SpriteRenderer::projectionMatRelative = projectionMatRelative; }
};