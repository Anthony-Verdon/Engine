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

void AudioManager::Play(const std::string &sound)
{
    ma_result result = ma_engine_play_sound(&engine, sound.c_str(), NULL);
    if (result != MA_SUCCESS)
        std::cerr << "failed to play " << sound << " : " << ma_result_description(result) << std::endl;
}

void AudioManager::SetVolume(float volume)
{
    ma_result result = ma_engine_set_volume(&engine, volume);
    if (result != MA_SUCCESS)
        std::cerr << "failed to set volume to " << volume << " : " << ma_result_description(result) << std::endl;
}