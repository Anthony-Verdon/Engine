#pragma once
#include <string>

// this class load and store texture for OpenGL usage
class Texture
{
  private:
    void GenerateTexture(unsigned char *data, int nrChannels);

    unsigned int ID;
    std::string path;
    int width;
    int height;

    Texture() = delete;
    Texture(const Texture &instance) = delete;
    Texture &operator=(const Texture &instance) = delete;

  public:
    Texture(const std::string &path);
    Texture(const unsigned char *buffer, size_t length);
    ~Texture();

    unsigned int getID() const { return ID; }
    std::string getPath() const { return path; }
    unsigned int getWidth() const { return width; }
    unsigned int getHeight() const { return height; }

    // theses functions load data from file or memory via stb_image library
    static unsigned char *LoadTextureFromFile(const std::string &path);
    static unsigned char *LoadTextureFromFile(const std::string &path, int &width, int &height, int &nrChannels);
    static unsigned char *LoadTextureFromMemory(const unsigned char *buffer, size_t length, int &width, int &height, int &nrChannels);
    static void UnloadTexture(unsigned char *data);
};