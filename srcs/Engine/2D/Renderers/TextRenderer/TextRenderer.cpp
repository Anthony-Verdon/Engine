#include "Engine/2D/Renderers/TextRenderer/TextRenderer.hpp"
#include "Engine/2D/Renderers/LineRenderer2D/LineRenderer2D.hpp"
#include "Engine/RessourceManager/RessourceManager.hpp"
#include "Engine/WindowManager/WindowManager.hpp"
#include "Engine/macros.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
unsigned int TextRenderer::VAO = -1;
unsigned int TextRenderer::VBO = -1;
bool TextRenderer::isInit = false;
FT_Library TextRenderer::ft;
std::map<std::string, std::vector<t_FreetypeCharacter>> TextRenderer::fonts;

void TextRenderer::Init()
{
    CHECK_AND_RETURN_VOID(!isInit, "TextRenderer already initialized");

    RessourceManager::AddShader("Text", PATH_TO_ENGINE "shaders/2D/text/text.vs", PATH_TO_ENGINE "shaders/2D/text/text.fs");
    std::shared_ptr<Shader> textShader = RessourceManager::GetShader("Text");
    textShader->use();
    textShader->setMat4("projection", ml::ortho(0.0f, (float)WindowManager::GetWindowWidth(), 0.0f, (float)WindowManager::GetWindowHeight(), -1.0f, 1.0f));

    CHECK_AND_RETURN_VOID(!FT_Init_FreeType(&ft), "FT_Init_FreeType failed");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    isInit = true;
}

void TextRenderer::LoadFont(const std::string &fontname, const std::string &path, int pixelSize)
{
    CHECK_AND_RETURN_VOID(isInit, "TextRenderer not initialized");

    CHECK((fonts.find(fontname) == fonts.end()), "font " + fontname + " already exist");

    FT_Face face;
    CHECK_AND_RETURN_VOID(!FT_New_Face(ft, path.c_str(), 0, &face), "FT_New_Face for " + path + " failed");

    FT_Set_Pixel_Sizes(face, 0, pixelSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            t_FreetypeCharacter freetypeCharacter;
            fonts[fontname].push_back(freetypeCharacter);
            std::cerr << "failed to load character " << c << " for " << fontname << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        t_FreetypeCharacter freetypeCharacter = {
            texture,
            ml::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            ml::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (unsigned int)face->glyph->advance.x,
        };
        fonts[fontname].push_back(freetypeCharacter);
    }
    FT_Done_Face(face);
}

void TextRenderer::Destroy()
{
    CHECK_AND_RETURN_VOID(isInit, "TextRenderer not initialized");

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    FT_Done_FreeType(ft);

    fonts.clear();

    isInit = false;
}

void TextRenderer::Draw(const std::string &text, const std::string &font, float x, float y, float scale, const ml::vec4 &color)
{
    CHECK_AND_RETURN_VOID(isInit, "TextRenderer not initialized");

    std::shared_ptr<Shader> textShader = RessourceManager::GetShader("Text");
    textShader->use();
    textShader->setVec4("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for (size_t i = 0; i < text.length(); i++)
    {
        unsigned char character = text[i];
        t_FreetypeCharacter freetypeCharacter = fonts[font][character];

        float xpos = x + freetypeCharacter.bearing.x * scale;
        float ypos = y - (freetypeCharacter.size.y - freetypeCharacter.bearing.y) * scale;

        float w = freetypeCharacter.size.x * scale;
        float h = freetypeCharacter.size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos, ypos, 0.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 1.0f},

            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos + w, ypos, 1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}};
        // render glyph texture over quad

        glBindTexture(GL_TEXTURE_2D, freetypeCharacter.textureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (freetypeCharacter.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}