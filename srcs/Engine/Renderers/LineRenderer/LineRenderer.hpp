#pragma once

#include <glm/glm.hpp>

class LineRenderer
{
    private:
        static unsigned int VAO;
        static unsigned int VBO;
        static bool isInit;

        static glm::mat4 projectionMatAbsolute;
        static glm::mat4 projectionMatRelative;

        LineRenderer() = delete;
        ~LineRenderer() = delete;
    public: 
        static void Init();
        static void Destroy();
        static void Draw(const glm::vec2 &va, const glm::vec2 &vb, const glm::vec3 &color, bool drawAbsolute = false);
        static void Draw(const glm::vec2 &va, const glm::vec2 &vb, const glm::vec4 &color, bool drawAbsolute = false); //@todo add something to change lineWidth

        static void SetProjectionMatRelative(const glm::mat4 &projectionMatRelative) { LineRenderer::projectionMatRelative = projectionMatRelative; }
};