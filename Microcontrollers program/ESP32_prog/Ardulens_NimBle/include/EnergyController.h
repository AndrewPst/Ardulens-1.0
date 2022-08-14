#pragma once

#include <Arduino.h>
#include <Events.h>
#include <Settings.h>

class EnergyController
{
private:
    uint32_t lastActiveTime_ = 0;
    bool saveMode_ = false;
public:

    Events<bool> onEnergySavingStateChanged;

    uint32_t getLastActiveTime();
    void updateLastActiveTime();
    void checkEnergySaveNeeding();

    void setEnergySave(bool mode);
    bool getPowerSaveMode();

    uint16_t getBattetyLevel();
};

extern EnergyController energy;