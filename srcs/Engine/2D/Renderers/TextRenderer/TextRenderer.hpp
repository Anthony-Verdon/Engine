#pragma once

#include <map>
#include "Matrix/Matrix.hpp"
#include <vector>

typedef struct s_FreetypeCharacter
{
    unsigned int textureID; // ID handle of the glyph texture
    ml::vec2 size;          // Size of glyph
    ml::vec2 bearing;       // Offset from baseline to left/top of glyph
    unsigned int advance;   // Offset to advance to next glyph
} t_FreetypeCharacter;

typedef struct FT_LibraryRec_ *FT_Library;

class TextRenderer
{
  private:
    static unsigned int VAO;
    static unsigned int VBO;

    static bool isInit;

    static FT_Library ft;
    static std::map<std::string, std::vector<t_FreetypeCharacter>> fonts;

    TextRenderer() = delete;
    ~TextRenderer() = delete;

  public:
    static void Init();
    static void LoadFont(const std::string &fontname, const std::string &path, int pixelSize);
    static void Destroy();
    static void Draw(const std::string &text, const std::string &font, float x, float y, float scale, const ml::vec4 &color);
};