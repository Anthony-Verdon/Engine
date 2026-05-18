#pragma once

#include "Matrix/Matrix.hpp"
#include <string>
#include <vector>
#include "Engine/2D/Sprite/Sprite.hpp"

struct SpriteRenderData
{
    ml::vec3 position;
    ml::vec2 spriteOffset;
    ml::vec2 boundingBox;
    ml::vec2 size;
    float rotation;
    ml::vec4 color;
    Sprite sprite;
    bool flipHorizontally;
    bool flipVertically;
    bool drawAbsolute;
    bool depthZ;

    SpriteRenderData()
    {
        position = ml::vec3(0, 0, 0);
        spriteOffset = ml::vec2(0, 0);
        boundingBox = ml::vec2(0, 0);
        size = ml::vec2(0, 0);
        rotation = 0;
        color = ml::vec4(1, 1, 1, 1);
        sprite = Sprite::none;
        flipHorizontally = false;
        flipVertically = false;
        drawAbsolute = false;
        depthZ = true;
    }
};

class SpriteRenderDataBuilder
{
  private:
    SpriteRenderData data;

  public:
    SpriteRenderDataBuilder() {}
    ~SpriteRenderDataBuilder() {}

    SpriteRenderDataBuilder &SetPosition(const ml::vec3 &position)
    {
        data.position = position;
        return *this;
    }
    SpriteRenderDataBuilder &SetSpriteOffset(const ml::vec2 &spriteOffset)
    {
        data.spriteOffset = spriteOffset;
        return *this;
    }
    SpriteRenderDataBuilder &SetBoundingBox(const ml::vec2 &boundingBox)
    {
        data.boundingBox = boundingBox;
        return *this;
    }
    SpriteRenderDataBuilder &SetSize(const ml::vec2 &size)
    {
        data.size = size;
        return *this;
    }
    SpriteRenderDataBuilder &SetRotation(float rotation)
    {
        data.rotation = rotation;
        return *this;
    }
    SpriteRenderDataBuilder &SetColor(const ml::vec3 &color)
    {
        data.color = ml::vec4(color.x, color.y, color.z, 1);
        return *this;
    }
    SpriteRenderDataBuilder &SetColor(const ml::vec4 &color)
    {
        data.color = color;
        return *this;
    }
    SpriteRenderDataBuilder &SetSprite(const Sprite &sprite)
    {
        data.sprite = sprite;
        return *this;
    }
    SpriteRenderDataBuilder &FlipHorizontally(bool flip)
    {
        data.flipHorizontally = flip;
        return *this;
    }
    SpriteRenderDataBuilder &FlipVertically(bool flip)
    {
        data.flipVertically = flip;
        return *this;
    }
    SpriteRenderDataBuilder &SetDrawAbsolute(bool drawAbsolute)
    {
        data.drawAbsolute = drawAbsolute;
        return *this;
    }
    SpriteRenderDataBuilder &SetDepthZ(bool depthZ)
    {
        data.depthZ = depthZ;
        return *this;
    }
    SpriteRenderData Build() { return data; }
};

class SpriteRenderer
{
  private:
    static unsigned int VAO;
    static unsigned int VBO;
    static bool isInit;

    static ml::mat4 projectionMatAbsolute;
    static ml::mat4 projectionMatRelative;

    static std::vector<SpriteRenderData> spritesToDraw;

    SpriteRenderer() = delete;
    ~SpriteRenderer() = delete;

  public:
    static void Init();
    static void Destroy();
    static void Draw(const SpriteRenderData &data);
    static void Draw();

    static void SetProjectionMatRelative(const ml::mat4 &projectionMatRelative) { SpriteRenderer::projectionMatRelative = projectionMatRelative; }
};