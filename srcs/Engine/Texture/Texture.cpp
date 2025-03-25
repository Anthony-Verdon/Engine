#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <filesystem>
#include <glad/glad.h>
#include <iostream>
#include <stb/stb_image.h>
#include <vector>

Texture::Texture(const std::string &path)
{
    this->path = path;

    if (std::filesystem::is_regular_file(path))
        loadTexture(path);
    else if (std::filesystem::is_directory(path)) //@todo erase skybox from lib
        loadSkybox(path);
}

Texture::~Texture()
{
}

void Texture::loadTexture(const std::string &path)
{
    int width; //@todo should save width, height
    int height;
    int nrChannels;
    //@todo check path
    stbi_set_flip_vertically_on_load(false);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

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

    this->width = width;
    this->height = height;
}

void Texture::loadSkybox(const std::string &path)
{
    std::vector<std::string> faces = {path + "right.jpg",  path + "left.jpg",  path + "top.jpg",
                                      path + "bottom.jpg", path + "front.jpg", path + "back.jpg"};

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    stbi_set_flip_vertically_on_load(false);

    int width;
    int height;
    int nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            unsigned int format = 0;
            if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE,
                         &data[0]);
        }
        else
            std::cerr << "Skybox tex failed to load at path: " << faces[i] << std::endl;
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}