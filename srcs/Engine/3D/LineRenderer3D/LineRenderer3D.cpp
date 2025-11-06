#include "Engine/3D/LineRenderer3D/LineRenderer3D.hpp"
#include "Engine/RessourceManager/RessourceManager.hpp"
#include "Engine/WindowManager/WindowManager.hpp"
#include "Engine/macros.hpp"
#include "Matrix/Matrix.hpp"

unsigned int LineRenderer3D::VAO = -1;
unsigned int LineRenderer3D::VBO = -1;
bool LineRenderer3D::isInit = false;
std::vector<float> LineRenderer3D::vertices;

void LineRenderer3D::Init()
{
    CHECK_AND_RETURN_VOID(!isInit, "LineRenderer3D already initialized");

    RessourceManager::AddShader("Line3D", PATH_TO_ENGINE "shaders/line3D.vs", PATH_TO_ENGINE "shaders/line3D.fs");
    std::shared_ptr<Shader> lineShader = RessourceManager::GetShader("Line3D");
    lineShader->use();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    float vertices[] = {0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0};

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0); // xyz
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(sizeof(float) * 3)); // rgba
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    isInit = true;
}

void LineRenderer3D::Destroy()
{
    CHECK_AND_RETURN_VOID(isInit, "LineRenderer3D not initialized");

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    isInit = false;
}

void LineRenderer3D::Draw(const ml::vec3 &va, const ml::vec3 &vb, const ml::vec3 &color)
{
    LineRenderer3D::Draw(va, vb, ml::vec4(color, 1));
}

void LineRenderer3D::Draw(const ml::vec3 &va, const ml::vec3 &vb, const ml::vec4 &color)
{
    for (size_t j = 0; j < 3; j++)
        vertices.push_back(va[j]);
    for (size_t j = 0; j < 4; j++)
        vertices.push_back(color[j]);

    for (size_t j = 0; j < 3; j++)
        vertices.push_back(vb[j]);
    for (size_t j = 0; j < 4; j++)
        vertices.push_back(color[j]);
}

void LineRenderer3D::Draw(const ml::mat4 &projection, const ml::mat4 &view)
{
    CHECK_AND_RETURN_VOID(isInit, "LineRenderer3D not initialized");

    std::shared_ptr<Shader> lineShader = RessourceManager::GetShader("Line3D");
    lineShader->use();
    lineShader->setMat4("projection", projection);
    lineShader->setMat4("view", view);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, vertices.size() / 7);

    glBindVertexArray(0);

    vertices.clear();
}
