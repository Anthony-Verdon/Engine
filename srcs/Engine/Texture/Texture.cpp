#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <filesystem>
#include <glad/glad.h>
#include <iostream>
#include <stb/stb_image.h>
#include <vector>

Texture::Texture(const std::string &path)
{
    this->path = path; //@todo check path
    int nrChannels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    GenerateTexture(data, width, height, nrChannels);
}

Texture::Texture(const unsigned char* buffer, size_t length)
{
    path = ""; //@todo, what should we do with this param in that case ?
    int nrChannels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char *data = stbi_load_from_memory(buffer, length, &width, &height, &nrChannels, 0);
    GenerateTexture(data, width, height, nrChannels);
}

Texture::~Texture()
{
}

void Texture::GenerateTexture(unsigned char *data, int width, int height, int nrChannels)
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

    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}