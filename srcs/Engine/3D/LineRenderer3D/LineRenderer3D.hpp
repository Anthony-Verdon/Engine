#pragma once

#include "Matrix/Matrix.hpp"
#include <vector>

class LineRenderer3D
{
    private:
        static unsigned int VAO;
        static unsigned int VBO;
        static bool isInit;
        static std::vector<float> vertices;

        LineRenderer3D() = delete;
        ~LineRenderer3D() = delete;
    public: 
        static void Init();
        static void Destroy();
        static void Draw(const ml::vec3 &va, const ml::vec3 &vb, const ml::vec3 &color);
        static void Draw(const ml::vec3 &va, const ml::vec3 &vb, const ml::vec4 &color);
        static void Draw(const ml::mat4 &projection, const ml::mat4 &view);
};