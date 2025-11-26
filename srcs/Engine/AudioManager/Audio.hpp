#pragma once

#include <string>
#include <miniaudio.h>

class AudioManager;

class Audio
{
  private:
    ma_sound sound;
    bool init;

  public:
    Audio();
    ~Audio();

    void Play(bool reset = true);
    void Stop();
    void SetLooping(bool looping);

    bool IsPlaying();
    bool IsLooping();
    bool AtEnd();

    friend AudioManager;
};