#pragma once
#include <Arduino.h>
#include <Events.h>

enum ButtonAction_t : uint8_t
{
    B_RELEASED,
    B_PRESSED,
    B_HOLDED,
    B_CLICK,
    B_HOLD,
    B_MULTICLICK
};

class TouchButton
{
    friend class ButtonsController;

public:
    TouchButton(uint8_t);

    void tick();

    ButtonAction_t getButtonAction();
    uint16_t getClicksCount();
    bool isNewAction();
    bool isPressed();
    bool isStateChanged();

private:
    ButtonAction_t actionType = B_RELEASED;
    ButtonAction_t lastType_ = B_RELEASED;

    const uint8_t pin_;
    uint32_t lastTouchTime_;
    uint32_t releaseTime_;
    bool lastTouchState_ = false;
    uint16_t clickCounter_ = 0;
    bool multiclickFlag_ = false;
    bool isNewAction_ = false;
    bool isStateChanged_ = false;
    bool lastPressState = false;
};