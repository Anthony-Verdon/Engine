#include "Engine/2D/Renderers/SpriteRenderer/SpriteRenderer.hpp"
#include "Engine/RessourceManager/RessourceManager.hpp"
#include "Engine/WindowManager/WindowManager.hpp"
#include <glad/glad.h>
#include <numeric>
#include <algorithm>
#include "Engine/macros.hpp"
#include "Engine/2D/Renderers/LineRenderer2D/LineRenderer2D.hpp"
unsigned int SpriteRenderer::VAO = -1;
unsigned int SpriteRenderer::VBO = -1;
bool SpriteRenderer::isInit = false;
ml::mat4 SpriteRenderer::projectionMatAbsolute;
ml::mat4 SpriteRenderer::projectionMatRelative;
std::vector<SpriteRenderData> SpriteRenderer::spritesToDraw = {};

void SpriteRenderer::Init()
{
    CHECK_AND_RETURN_VOID(!isInit, "SpriteRenderer already initialized");

    RessourceManager::AddShader("Sprite", PATH_TO_ENGINE "shaders/2D/sprite/sprite.vs", PATH_TO_ENGINE "shaders/2D/sprite/sprite.fs");
    std::shared_ptr<Shader> spriteShader = RessourceManager::GetShader("Sprite");
    if (!spriteShader)
        return;
    spriteShader->use();
    spriteShader->setInt("image", 0);
    projectionMatAbsolute = ml::ortho(0.0f, (float)WindowManager::GetWindowWidth(), (float)WindowManager::GetWindowHeight(), 0.0f, -1.0f, 1.0f);
    spriteShader->setMat4("projection", projectionMatAbsolute);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    float positions[] = {
        -0.5f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f,

        -0.5f, 0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f};

    float textures[] = {
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f};

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(textures), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(textures), textures);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)sizeof(positions));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    isInit = true;
}

void SpriteRenderer::Destroy()
{
    CHECK_AND_RETURN_VOID(isInit, "SpriteRenderer not initialized");

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void SpriteRenderer::Draw(const SpriteRenderData &data)
{
    spritesToDraw.push_back(data);
}

void SpriteRenderer::Draw()
{
    CHECK_AND_RETURN_VOID(isInit, "SpriteRenderer not initialized");

    std::vector<int> index(spritesToDraw.size());
    std::iota(index.begin(), index.end(), 0);

    std::sort(index.begin(), index.end(),
              [](int a, int b) {
                  if (spritesToDraw[a].depthZ && spritesToDraw[b].depthZ)
                  {

                      ml::vec3 positionSpriteA = spritesToDraw[a].position - ml::vec3(spritesToDraw[a].boundingBox / 2, 0);
                      ml::vec3 positionSpriteB = spritesToDraw[b].position - ml::vec3(spritesToDraw[b].boundingBox / 2, 0);
                      if (positionSpriteA.z != positionSpriteB.z)
                          return positionSpriteA.z < positionSpriteB.z;
                      else if (positionSpriteA.y != positionSpriteB.y)
                          return positionSpriteA.y < positionSpriteB.y;
                      else
                          return positionSpriteA.x < positionSpriteB.x;
                  }
                  else if (spritesToDraw[a].depthZ)
                      return (true);
                  else if (spritesToDraw[b].depthZ)
                      return (false);
                  else
                      return (a < b);
              });

    for (size_t i = 0; i < index.size(); i++)
    {
        if (!RessourceManager::TextureExist(spritesToDraw[index[i]].sprite.textureName))
        {
            std::cerr << "texture " << spritesToDraw[index[i]].sprite.textureName << " not loaded" << std::endl;
            continue;
        }
        ml::vec2 TopLeftCoords;
        ml::vec2 BotomRightCoords;
        TopLeftCoords.x = 1.0f / spritesToDraw[index[i]].sprite.textureSize.x * spritesToDraw[index[i]].sprite.spriteCoords.x;
        TopLeftCoords.y = 1.0f / spritesToDraw[index[i]].sprite.textureSize.y * spritesToDraw[index[i]].sprite.spriteCoords.y;
        BotomRightCoords.x = 1.0f / spritesToDraw[index[i]].sprite.textureSize.x * (spritesToDraw[index[i]].sprite.spriteCoords.x + 1);
        BotomRightCoords.y = 1.0f / spritesToDraw[index[i]].sprite.textureSize.y * (spritesToDraw[index[i]].sprite.spriteCoords.y + 1);

        if (spritesToDraw[index[i]].flipHorizontally)
            std::swap(TopLeftCoords.x, BotomRightCoords.x);
        if (spritesToDraw[index[i]].flipVertically)
            std::swap(TopLeftCoords.y, BotomRightCoords.y);

        float positions[] = {
            -0.5f, 0.5f,
            0.5f, -0.5f,
            -0.5f, -0.5f,

            -0.5f, 0.5f,
            0.5f, 0.5f,
            0.5f, -0.5f};

        float textures[] = {
            TopLeftCoords.x, BotomRightCoords.y,
            BotomRightCoords.x, TopLeftCoords.y,
            TopLeftCoords.x, TopLeftCoords.y,

            TopLeftCoords.x, BotomRightCoords.y,
            BotomRightCoords.x, BotomRightCoords.y,
            BotomRightCoords.x, TopLeftCoords.y};

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(textures), textures);

        std::shared_ptr<Shader> spriteShader = RessourceManager::GetShader("Sprite");
        spriteShader->use();

        ml::mat4 model = ml::mat4(1.0f);
        model = ml::translate(model, spritesToDraw[index[i]].position - ml::vec3(spritesToDraw[index[i]].spriteOffset, 0));
        model = ml::rotate(model, ml::radians(spritesToDraw[index[i]].rotation), ml::vec3(0.0f, 0.0f, 1.0f));
        model = ml::scale(model, ml::vec3(spritesToDraw[index[i]].size, 1.0f));

        spriteShader->setMat4("model", model);
        spriteShader->setVec4("spriteColor", spritesToDraw[index[i]].color);
        if (spritesToDraw[index[i]].drawAbsolute)
            spriteShader->setMat4("projection", projectionMatAbsolute);
        else
            spriteShader->setMat4("projection", projectionMatRelative);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, RessourceManager::GetTexture(spritesToDraw[index[i]].sprite.textureName)->getID()); //@todo check
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        LineRenderer2D::Draw(ml::vec2(spritesToDraw[index[i]].position.x - spritesToDraw[index[i]].boundingBox.x / 2, spritesToDraw[index[i]].position.y - spritesToDraw[index[i]].boundingBox.y / 2), ml::vec2(spritesToDraw[index[i]].position.x + spritesToDraw[index[i]].boundingBox.x / 2, spritesToDraw[index[i]].position.y - spritesToDraw[index[i]].boundingBox.y / 2), ml::vec3(1, 0, 0), true);
        LineRenderer2D::Draw(ml::vec2(spritesToDraw[index[i]].position.x - spritesToDraw[index[i]].boundingBox.x / 2, spritesToDraw[index[i]].position.y - spritesToDraw[index[i]].boundingBox.y / 2), ml::vec2(spritesToDraw[index[i]].position.x - spritesToDraw[index[i]].boundingBox.x / 2, spritesToDraw[index[i]].position.y + spritesToDraw[index[i]].boundingBox.y / 2), ml::vec3(1, 0, 0), true);
        LineRenderer2D::Draw(ml::vec2(spritesToDraw[index[i]].position.x - spritesToDraw[index[i]].boundingBox.x / 2, spritesToDraw[index[i]].position.y + spritesToDraw[index[i]].boundingBox.y / 2), ml::vec2(spritesToDraw[index[i]].position.x + spritesToDraw[index[i]].boundingBox.x / 2, spritesToDraw[index[i]].position.y + spritesToDraw[index[i]].boundingBox.y / 2), ml::vec3(1, 0, 0), true);
        LineRenderer2D::Draw(ml::vec2(spritesToDraw[index[i]].position.x + spritesToDraw[index[i]].boundingBox.x / 2, spritesToDraw[index[i]].position.y - spritesToDraw[index[i]].boundingBox.y / 2), ml::vec2(spritesToDraw[index[i]].position.x + spritesToDraw[index[i]].boundingBox.x / 2, spritesToDraw[index[i]].position.y + spritesToDraw[index[i]].boundingBox.y / 2), ml::vec3(1, 0, 0), true);
    }

    spritesToDraw.clear();

    glBindVertexArray(0);
}