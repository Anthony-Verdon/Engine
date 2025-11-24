#pragma once

#include <string>
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
    static void SetVolume(float volume);
};