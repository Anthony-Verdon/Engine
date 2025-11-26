#pragma once

#include "Engine/AudioManager/Audio.hpp"
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

    static Audio InitSound(const std::string &path);
    static void InitSound(Audio &audio, const std::string &path);
    static void Play(const std::string &path);
    static void SetVolume(float volume);
};