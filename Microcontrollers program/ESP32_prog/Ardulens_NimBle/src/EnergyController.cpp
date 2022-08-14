#include <EnergyController.h>

EnergyController energy = EnergyController();

uint32_t EnergyController::getLastActiveTime()
{
    return lastActiveTime_;
}

void EnergyController::updateLastActiveTime()
{
    lastActiveTime_ = millis();
    if (saveMode_)
    {
        saveMode_ = false;
        onEnergySavingStateChanged.Invoke(saveMode_);
    }
}

void EnergyController::checkEnergySaveNeeding()
{
    if(saveMode_) return;
    if(settings.getValueByKey(SETUP_SHUTDOWN_TIME) == 0) return;
    if ((millis() - lastActiveTime_) / 1000 > settings.getValueByKey(SETUP_SHUTDOWN_TIME))
    {
        saveMode_ = true;
        onEnergySavingStateChanged.Invoke(saveMode_);
    }
}

bool EnergyController::getPowerSaveMode()
{
    return saveMode_;
}