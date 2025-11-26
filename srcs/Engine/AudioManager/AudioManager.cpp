#include "Engine/AudioManager/AudioManager.hpp"
#include <iostream>

ma_engine AudioManager::engine;

void AudioManager::Init()
{
    ma_result result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS)
        std::cerr << "failed to init audio engine" << ma_result_description(result) << std::endl;
}

void AudioManager::Destroy()
{
    ma_engine_uninit(&engine);
}

Audio AudioManager::InitSound(const std::string &path)
{
    Audio audio;

    InitSound(audio, path);

    return audio;
}

void AudioManager::InitSound(Audio &audio, const std::string &path)
{
    ma_result result = ma_sound_init_from_file(&engine, path.c_str(), 0, NULL, NULL, &audio.sound);
    if (result != MA_SUCCESS)
        std::cerr << "failed to init sound " << path << " : " << ma_result_description(result) << std::endl;
    else
        audio.init = true;
}

void AudioManager::Play(const std::string &path)
{
    ma_result result = ma_engine_play_sound(&engine, path.c_str(), NULL);
    if (result != MA_SUCCESS)
        std::cerr << "failed to play " << path << " : " << ma_result_description(result) << std::endl;
}

void AudioManager::SetVolume(float volume)
{
    ma_result result = ma_engine_set_volume(&engine, volume);
    if (result != MA_SUCCESS)
        std::cerr << "failed to set volume to " << volume << " : " << ma_result_description(result) << std::endl;
}