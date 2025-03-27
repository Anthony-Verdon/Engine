#pragma once

#include "Matrix/Matrix.hpp"

#include <unordered_map>
#include <string>
#include <vector>

struct PolygonGl
{
    unsigned int VAO;
    unsigned int EBO;
    std::vector<float> vertices;
    unsigned int nbFaces;
};

class PolygonRenderer
{
    private:
        static std::unordered_map<std::string, PolygonGl> polygons;
        static bool isInit;
        
        static ml::mat4 projectionMatAbsolute;
        static ml::mat4 projectionMatRelative;
        
        PolygonRenderer() = delete;
        ~PolygonRenderer() = delete;
    public: 
        static void Init();
        static void Destroy();
        static void LoadPolygon(const std::string &polygonName, const std::vector<float> &vertices, const std::vector<int> &faces);
        static void Draw(const std::string &polygonName, const ml::vec2 &position, const ml::vec2 &size, float rotation, const ml::vec3 &fillColor, const ml::vec3 &edgeColor = ml::vec3(0, 0, 0), bool drawAbsolute = false);
        static void Draw(const std::string &polygonName, const ml::vec2 &position, const ml::vec2 &size, float rotation, const ml::vec4 &fillColor, const ml::vec4 &edgeColor = ml::vec4(0, 0, 0, 0), bool drawAbsolute = false);

        static void SetProjectionMatRelative(const ml::mat4 &projectionMatRelative) { PolygonRenderer::projectionMatRelative = projectionMatRelative; }
};