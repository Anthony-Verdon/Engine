#pragma once
#include <string>

class Texture
{
  private:
    void GenerateTexture(unsigned char *data, int width, int height, int nrChannels);

    unsigned int ID;
    std::string path;
    int width;
    int height;

    Texture() = delete;
    Texture(const Texture &instance) = delete;
    Texture &operator=(const Texture &instance) = delete;

  public:
    Texture(const std::string &path);
    Texture(const unsigned char* buffer, size_t length);
    ~Texture();

    unsigned int getID() const {return ID; }
    std::string getPath() const {return path; }
    unsigned int getWidth() const {return width; }
    unsigned int getHeight() const {return height; }
};