#include "Texture.hpp"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture(const std::string &path)
{
    this->path = path; //@todo check path
    int nrChannels;
    unsigned char *data = LoadTextureFromFile(path, width, height, nrChannels);
    GenerateTexture(data, nrChannels);
}

Texture::Texture(const unsigned char *buffer, size_t length)
{
    path = ""; //@todo, what should we do with this param in that case ?
    int nrChannels;
    unsigned char *data = LoadTextureFromMemory(buffer, length, width, height, nrChannels);
    GenerateTexture(data, nrChannels);
}

Texture::~Texture()
{
}

void Texture::GenerateTexture(unsigned char *data, int nrChannels)
{
    unsigned int format = 0;
    if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, &data[0]);
    glGenerateMipmap(GL_TEXTURE_2D);

    UnloadTexture(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

unsigned char *Texture::LoadTextureFromFile(const std::string &path)
{
    int width, height, nrChannels;
    return (LoadTextureFromFile(path, width, height, nrChannels));
}

unsigned char *Texture::LoadTextureFromFile(const std::string &path, int &width, int &height, int &nrChannels)
{
    stbi_set_flip_vertically_on_load(false);
    return stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
}

unsigned char *Texture::LoadTextureFromMemory(const unsigned char *buffer, size_t length, int &width, int &height, int &nrChannels)
{
    stbi_set_flip_vertically_on_load(false);
    return (stbi_load_from_memory(buffer, length, &width, &height, &nrChannels, 0));
}

void Texture::UnloadTexture(unsigned char *data)
{
    stbi_image_free(data);
}