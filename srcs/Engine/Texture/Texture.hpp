#pragma once

#include <memory>
#include <string>

class Texture
{
  private:
    Texture() = delete;
    void checkHeader(const std::string &line, unsigned int nbLine);
    void loadImage(const std::string &texturePath);

    unsigned int ID;
    int width;
    int height;
    unsigned int valueMax;
    std::unique_ptr<unsigned char[]> data;

    static bool textureInit;
    class Exception : public std::exception
    {
      public:
        Exception(const std::string &functionName, const std::string &errorMessage, const std::string &line,
                  unsigned int lineIndex);
        const char *what(void) const throw();

      private:
        std::string errorMessage;
    };

  public:
    Texture(const std::string &texturePath);
    Texture(const unsigned char* buffer, size_t length);
    Texture(const Texture &copy);
    Texture &operator=(const Texture &copy);
    ~Texture();

    static void initTexParameter();

    unsigned int getID() const;
};