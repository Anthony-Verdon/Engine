#pragma once

namespace UI
{

struct EventData
{
    unsigned int componentID;
    unsigned int event;

    EventData(unsigned int event) : event(event) {}
    virtual ~EventData() {}
};

enum EngineEvents
{
    CURSEUR_ON = 0,
    CURSEUR_OFF,
    CLICK_ON,
    CLICK_OFF,
    UPDATE_VALUE,

    MAX_ENGINE_EVENTS
};

}; // namespace UI