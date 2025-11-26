#include "Engine/AudioManager/Audio.hpp"
#include "Engine/macros.hpp"

Audio::Audio()
{
    init = false;
}

Audio::~Audio()
{
    if (init)
        ma_sound_uninit(&sound);
    init = false;
}

void Audio::Play(bool reset)
{
    CHECK_AND_RETURN_VOID(init, "Audio not init");

    if (reset)
        ma_sound_seek_to_pcm_frame(&sound, 0);
    ma_sound_start(&sound);
}

void Audio::Stop()
{
    CHECK_AND_RETURN_VOID(init, "Audio not init");
    ma_sound_stop(&sound);
}

void Audio::SetLooping(bool looping)
{
    CHECK_AND_RETURN_VOID(init, "Audio not init");
    ma_sound_set_looping(&sound, looping);
}

bool Audio::IsPlaying()
{
    CHECK_AND_RETURN(init, false, "Audio not init");
    return ma_sound_is_playing(&sound);
}

bool Audio::IsLooping()
{
    CHECK_AND_RETURN(init, false, "Audio not init");
    return ma_sound_is_looping(&sound);
}

bool Audio::AtEnd()
{
    CHECK_AND_RETURN(init, false, "Audio not init");
    return ma_sound_at_end(&sound);
}