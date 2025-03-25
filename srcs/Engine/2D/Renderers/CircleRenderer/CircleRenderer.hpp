#pragma once

#include "vec/vec.hpp"
#include "mat/mat.hpp"
#include <vector>

class CircleRenderer
{
     private:
        static unsigned int VAO;
        static unsigned int nbTriangles;
        static bool isInit;
        static std::vector<float> vertices;

        static ml::mat4 projectionMatAbsolute;
        static ml::mat4 projectionMatRelative;

        CircleRenderer() = delete;
        ~CircleRenderer() = delete;
    public: 
        static void Init();
        static void Destroy();
        static void Draw(const ml::vec2 &position, float radius, float rotation, const ml::vec3 &fillColor, const ml::vec3 &edgeColor = ml::vec3(0, 0, 0), bool drawAbsolute = false);
        static void Draw(const ml::vec2 &position, float radius, float rotation, const ml::vec4 &fillColor, const ml::vec4 &edgeColor = ml::vec4(0, 0, 0, 0), bool drawAbsolute = false);

        static void SetProjectionMatRelative(const ml::mat4 &projectionMatRelative) { CircleRenderer::projectionMatRelative = projectionMatRelative; }
};