#include "Engine/2D/Renderers/PolygonRenderer/PolygonRenderer.hpp"
#include "Engine/2D/Renderers/LineRenderer/LineRenderer.hpp"
#include "Engine/RessourceManager/RessourceManager.hpp"
#include "Engine/WindowManager/WindowManager.hpp"
#include "Engine/macros.hpp"


std::unordered_map<std::string, PolygonGl> PolygonRenderer::polygons;
bool PolygonRenderer::isInit = false;
ml::mat4 PolygonRenderer::projectionMatAbsolute;
ml::mat4 PolygonRenderer::projectionMatRelative;

void PolygonRenderer::Init()
{
    CHECK_AND_RETURN_VOID(!isInit, "PolygonRenderer already initialized");

    RessourceManager::AddShader("Polygon", "shaders/polygon/polygon.vs", "shaders/polygon/polygon.fs");
    std::shared_ptr<Shader> polygonShader = RessourceManager::GetShader("Polygon");
    polygonShader->use();
    projectionMatAbsolute = ml::ortho(0.0f, (float)WindowManager::GetWindowWidth(), (float)WindowManager::GetWindowHeight(), 0.0f, -1.0f, 1.0f);
    polygonShader->setMat4("projection", projectionMatAbsolute);

    isInit = true;
}

void PolygonRenderer::Destroy()
{
    CHECK_AND_RETURN_VOID(isInit, "PolygonRenderer not initialized");

    for (auto it = polygons.begin(); it != polygons.end(); it++)
    {
        glDeleteVertexArrays(1, &(it->second.VAO));
        glDeleteBuffers(1, &(it->second.EBO));
    }
}

void PolygonRenderer::LoadPolygon(const std::string &polygonName, const std::vector<float> &vertices, const std::vector<int> &faces)
{
    CHECK_AND_RETURN_VOID(isInit, "PolygonRenderer not initialized");
    CHECK_AND_RETURN_VOID((polygons.find(polygonName) == polygons.end()), polygonName + " already initialized into PolygonRenderer");

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                 
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * faces.size(), faces.data(), GL_STATIC_DRAW); 

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);

    PolygonGl newPolygon;
    newPolygon.VAO = VAO;
    newPolygon.EBO = EBO;
    newPolygon.vertices = vertices;
    newPolygon.nbFaces = faces.size();
    polygons[polygonName] = newPolygon;
}

void PolygonRenderer::Draw(const std::string &polygonName, const ml::vec2 &position, const ml::vec2 &size, float rotation, const ml::vec3 &fillColor, const ml::vec3 &edgeColor, bool drawAbsolute)
{
    PolygonRenderer::Draw(polygonName, position, size, rotation, ml::vec4(fillColor, 1), ml::vec4(edgeColor, 1), drawAbsolute);
}

void PolygonRenderer::Draw(const std::string &polygonName, const ml::vec2 &position, const ml::vec2 &size, float rotation, const ml::vec4 &fillColor, const ml::vec4 &edgeColor, bool drawAbsolute)
{
    CHECK_AND_RETURN_VOID(isInit, "PolygonRenderer not initialized");
    CHECK_AND_RETURN_VOID((polygons.find(polygonName) != polygons.end()), polygonName + " not found into PolygonRenderer");

    if (fillColor.w != 0)
    {
        std::shared_ptr<Shader> polygonShader = RessourceManager::GetShader("Polygon");
        polygonShader->use();

        ml::mat4 model = ml::mat4(1.0f);
        model = ml::translate(model, ml::vec3(position, 0.0f));  
        model = ml::rotate(model, ml::radians(rotation), ml::vec3(0.0f, 0.0f, 1.0f)); 
        model = ml::scale(model, ml::vec3(size, 1.0f)); 

        polygonShader->setMat4("model", model);
        polygonShader->setVec4("color", fillColor);
        if (drawAbsolute)
            polygonShader->setMat4("projection", projectionMatAbsolute);
        else
            polygonShader->setMat4("projection", projectionMatRelative);

        PolygonGl polygon = polygons[polygonName];
        glBindVertexArray(polygon.VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, polygon.EBO);
        glDrawElements(GL_TRIANGLES, polygon.nbFaces, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    if (edgeColor.w != 0)
    {
        PolygonGl polygon = polygons[polygonName];
        for (unsigned int i = 0; i < polygon.vertices.size(); i += 2)
        {
            // @todo move this part into lineRenderer draw because it's duplicated from DrawSolidPolygonFcn()
            ml::vec2 va = {polygon.vertices[i], polygon.vertices[i + 1]};
            ml::vec2 vb = {polygon.vertices[(i + 2) % polygon.vertices.size()], polygon.vertices[(i + 3) % polygon.vertices.size()]} ;
            vb = vb * size;
            va = va * size;
            float x;
            float y;
            x = va.x * cos(ml::radians(rotation)) - va.y * sin(ml::radians(rotation));
            y = va.x * sin(ml::radians(rotation)) + va.y * cos(ml::radians(rotation));
            va = ml::vec2(x, y) + position;
            x = vb.x * cos(ml::radians(rotation)) - vb.y * sin(ml::radians(rotation));
            y = vb.x * sin(ml::radians(rotation)) + vb.y * cos(ml::radians(rotation));
            vb = ml::vec2(x, y) + position;
            LineRenderer::Draw(va, vb, edgeColor); //@todo should be init in PolygonRenderer in case it's not done before
        }
    }
}