#define _USE_MATH_DEFINES
#include "Engine/Renderers/CircleRenderer/CircleRenderer.hpp"
#include "Engine/Renderers/LineRenderer/LineRenderer.hpp"
#include "Engine/RessourceManager/RessourceManager.hpp"
#include "Engine/WindowManager/WindowManager.hpp"
#include <vector>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Engine/macros.hpp"

unsigned int CircleRenderer::VAO = -1;
unsigned int CircleRenderer::nbTriangles = 30;
bool CircleRenderer::isInit = false;
std::vector<float> CircleRenderer::vertices;
glm::mat4 CircleRenderer::projectionMatAbsolute;
glm::mat4 CircleRenderer::projectionMatRelative;

void CircleRenderer::Init()
{
    CHECK_AND_RETURN_VOID(!isInit, "CircleRenderer already initialized");

    RessourceManager::AddShader("Circle", "shaders/circle/circle.vs", "shaders/circle/circle.fs");
    std::shared_ptr<Shader> circleShader = RessourceManager::GetShader("Circle");
    circleShader->use();
    projectionMatAbsolute = glm::ortho(0.0f, (float)WindowManager::GetWindowWidth(), (float)WindowManager::GetWindowHeight(), 0.0f, -1.0f, 1.0f);
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

void CircleRenderer::Draw(const glm::vec2 &position, float radius, float rotation, const glm::vec3 &fillColor, const glm::vec3 &edgeColor, bool drawAbsolute)
{
    CircleRenderer::Draw(position, radius, rotation, glm::vec4(fillColor, 1), glm::vec4(edgeColor, 1), drawAbsolute);
}
void CircleRenderer::Draw(const glm::vec2 &position, float radius, float rotation, const glm::vec4 &fillColor, const glm::vec4 &edgeColor, bool drawAbsolute)
{
    CHECK_AND_RETURN_VOID(isInit, "CircleRenderer not initialized");
    
    if (fillColor.w != 0)
    {
        std::shared_ptr<Shader> circleShader = RessourceManager::GetShader("Circle");

        circleShader->use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position, 0.0f));  
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); 
        model = glm::scale(model, glm::vec3(radius, radius, 1.0f)); 

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
            glm::vec2 va = {vertices[i + 2], vertices[i + 3]};
            glm::vec2 vb = {vertices[i + 4], vertices[i + 5]};
            va = va * radius + position;
            vb = vb * radius + position;

            LineRenderer::Draw(va, vb, edgeColor, drawAbsolute);
        }
    }
}