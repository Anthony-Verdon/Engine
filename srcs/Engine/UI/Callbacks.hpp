#pragma once

namespace UI
{

struct CallbackData
{
    unsigned int callback;

    CallbackData(unsigned int callback) : callback(callback) {}
    virtual ~CallbackData() {}
};

enum EngineCallbacks
{
    CURSEUR_ON = 0,
    CURSEUR_OFF,
    CLICK_ON,
    CLICK_OFF,
    UPDATE_VALUE,

    MAX_ENGINE_CALLBACKS
};

}; // namespace UI