#pragma once

#include <string>
#define MINIAUDIO_IMPLEMENTATION
#define MA_ENABLE_VORBIS
#include <miniaudio.h>

class AudioManager
{
  private:
    AudioManager() = delete;
    ~AudioManager() = delete;

    static ma_engine engine;

  public:
    static void Init();
    static void Destroy();

    static void Play(const std::string &sound);
};