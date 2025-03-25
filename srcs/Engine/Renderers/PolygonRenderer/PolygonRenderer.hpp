#pragma once

#include <glm/glm.hpp>
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
        
        static glm::mat4 projectionMatAbsolute;
        static glm::mat4 projectionMatRelative;
        
        PolygonRenderer() = delete;
        ~PolygonRenderer() = delete;
    public: 
        static void Init();
        static void Destroy();
        static void LoadPolygon(const std::string &polygonName, const std::vector<float> &vertices, const std::vector<int> &faces);
        static void Draw(const std::string &polygonName, const glm::vec2 &position, const glm::vec2 &size, float rotation, const glm::vec3 &fillColor, const glm::vec3 &edgeColor = glm::vec3(0, 0, 0), bool drawAbsolute = false);
        static void Draw(const std::string &polygonName, const glm::vec2 &position, const glm::vec2 &size, float rotation, const glm::vec4 &fillColor, const glm::vec4 &edgeColor = glm::vec4(0, 0, 0, 0), bool drawAbsolute = false);

        static void SetProjectionMatRelative(const glm::mat4 &projectionMatRelative) { PolygonRenderer::projectionMatRelative = projectionMatRelative; }
};