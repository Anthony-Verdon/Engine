#pragma once

#include "Matrix/Matrix.hpp"


class LineRenderer2D
{
    private:
        static unsigned int VAO;
        static unsigned int VBO;
        static bool isInit;

        static ml::mat4 projectionMatAbsolute;
        static ml::mat4 projectionMatRelative;

        LineRenderer2D() = delete;
        ~LineRenderer2D() = delete;
    public: 
        static void Init();
        static void Destroy();
        static void Draw(const ml::vec2 &va, const ml::vec2 &vb, const ml::vec3 &color, bool drawAbsolute = false);
        static void Draw(const ml::vec2 &va, const ml::vec2 &vb, const ml::vec4 &color, bool drawAbsolute = false); //@todo add something to change lineWidth

        static void SetProjectionMatRelative(const ml::mat4 &projectionMatRelative) { LineRenderer2D::projectionMatRelative = projectionMatRelative; }
};