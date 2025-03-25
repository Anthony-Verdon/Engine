#define _USE_MATH_DEFINES
#include "Engine/2D/Renderers/CircleRenderer/CircleRenderer.hpp"
#include "Engine/2D/Renderers/LineRenderer/LineRenderer.hpp"
#include "Engine/RessourceManager/RessourceManager.hpp"
#include "Engine/WindowManager/WindowManager.hpp"
#include <vector>
#include <cmath>
#include "geometry/geometry.hpp"
#include "Engine/macros.hpp"

unsigned int CircleRenderer::VAO = -1;
unsigned int CircleRenderer::nbTriangles = 30;
bool CircleRenderer::isInit = false;
std::vector<float> CircleRenderer::vertices;
ml::mat4 CircleRenderer::projectionMatAbsolute;
ml::mat4 CircleRenderer::projectionMatRelative;

void CircleRenderer::Init()
{
    CHECK_AND_RETURN_VOID(!isInit, "CircleRenderer already initialized");

    RessourceManager::AddShader("Circle", "shaders/circle/circle.vs", "shaders/circle/circle.fs");
    std::shared_ptr<Shader> circleShader = RessourceManager::GetShader("Circle");
    circleShader->use();
    projectionMatAbsolute = ml::ortho(0.0f, (float)WindowManager::GetWindowWidth(), (float)WindowManager::GetWindowHeight(), 0.0f, -1.0f, 1.0f);
    circleShader->setMat4("projection", projectionMatAbsolute);

    // define circle data
    unsigned int radius = 1;

    // calculate vertices
    float prevX = radius * sin(0);
    float prevY = radius * cos(0);
    float angle = M_PI * 2.0 / nbTriangles;
    
    for (unsigned int i = 0; i <= nbTriangles; i++)
    {
        float newX = radius * sin(angle * i);
        float newY = radius * cos(angle * i);
        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(prevX);
        vertices.push_back(prevY);
        vertices.push_back(newX);
        vertices.push_back(newY);

        prevX = newX;
        prevY = newY;
    }

    // create mesh
    unsigned int VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);

    isInit = true;
}

void CircleRenderer::Destroy()
{
    CHECK_AND_RETURN_VOID(isInit, "CircleRenderer not initialized");

    glDeleteVertexArrays(1, &VAO);
}

void CircleRenderer::Draw(const ml::vec2 &position, float radius, float rotation, const ml::vec3 &fillColor, const ml::vec3 &edgeColor, bool drawAbsolute)
{
    CircleRenderer::Draw(position, radius, rotation, ml::vec4(fillColor, 1), ml::vec4(edgeColor, 1), drawAbsolute);
}
void CircleRenderer::Draw(const ml::vec2 &position, float radius, float rotation, const ml::vec4 &fillColor, const ml::vec4 &edgeColor, bool drawAbsolute)
{
    CHECK_AND_RETURN_VOID(isInit, "CircleRenderer not initialized");
    
    if (fillColor.w != 0)
    {
        std::shared_ptr<Shader> circleShader = RessourceManager::GetShader("Circle");

        circleShader->use();
        ml::mat4 model = ml::mat4(1.0f);
        model = ml::translate(model, ml::vec3(position, 0.0f));  
        model = ml::rotate(model, ml::radians(rotation), ml::vec3(0.0f, 0.0f, 1.0f)); 
        model = ml::scale(model, ml::vec3(radius, radius, 1.0f)); 

        circleShader->setMat4("model", model);
        circleShader->setVec4("color", fillColor);
        if (drawAbsolute)
            circleShader->setMat4("projection", projectionMatAbsolute);
        else
            circleShader->setMat4("projection", projectionMatRelative);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, (nbTriangles + 1) * 3);
        glBindVertexArray(0);
    }
    
    if (edgeColor.w != 0)
    {
        for (unsigned int i = 0; i < vertices.size(); i += 6)
        {
            ml::vec2 va = {vertices[i + 2], vertices[i + 3]};
            ml::vec2 vb = {vertices[i + 4], vertices[i + 5]};
            va = va * radius + position;
            vb = vb * radius + position;

            LineRenderer::Draw(va, vb, edgeColor, drawAbsolute);
        }
    }
}