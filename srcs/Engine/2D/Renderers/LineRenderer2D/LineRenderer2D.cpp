#include "Engine/2D/Renderers/LineRenderer2D/LineRenderer2D.hpp"
#include "Engine/RessourceManager/RessourceManager.hpp"
#include "Engine/WindowManager/WindowManager.hpp"
#include "Engine/macros.hpp"
#include "Matrix/Matrix.hpp"

unsigned int LineRenderer2D::VAO = -1;
unsigned int LineRenderer2D::VBO = -1;
bool LineRenderer2D::isInit = false;
ml::mat4 LineRenderer2D::projectionMatAbsolute;
ml::mat4 LineRenderer2D::projectionMatRelative;

void LineRenderer2D::Init()
{
    CHECK_AND_RETURN_VOID(!isInit, "LineRenderer2D already initialized");

    RessourceManager::AddShader("Line", PATH_TO_ENGINE "shaders/2D/line/line.vs", PATH_TO_ENGINE "shaders/2D/line/line.fs");
    std::shared_ptr<Shader> lineShader = RessourceManager::GetShader("Line");
    lineShader->use();
    projectionMatAbsolute = ml::ortho(0.0f, (float)WindowManager::GetWindowWidth(), (float)WindowManager::GetWindowHeight(), 0.0f, -1.0f, 1.0f);
    lineShader->setMat4("projection", projectionMatAbsolute);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    float vertices[] = {0, 0,
                        0, 0};

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    isInit = true;
}

void LineRenderer2D::Destroy()
{
    CHECK_AND_RETURN_VOID(isInit, "LineRenderer2D not initialized");

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    isInit = false;
}

void LineRenderer2D::Draw(const ml::vec2 &va, const ml::vec2 &vb, const ml::vec3 &color, bool drawAbsolute)
{
    LineRenderer2D::Draw(va, vb, ml::vec4(color, 1), drawAbsolute);
}

void LineRenderer2D::Draw(const ml::vec2 &va, const ml::vec2 &vb, const ml::vec4 &color, bool drawAbsolute)
{
    CHECK_AND_RETURN_VOID(isInit, "LineRenderer2D not initialized");

    std::shared_ptr<Shader> lineShader = RessourceManager::GetShader("Line");
    lineShader->use();
    lineShader->setVec4("color", color);
    if (drawAbsolute)
        lineShader->setMat4("projection", projectionMatAbsolute);
    else
        lineShader->setMat4("projection", projectionMatRelative);

    float vertices[] = {va.x, va.y,
                        vb.x, vb.y};

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}