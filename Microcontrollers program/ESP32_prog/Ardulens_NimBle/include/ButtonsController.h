#pragma once

#include <Arduino.h>

#include <Button.h>
#include <Events.h>
#include <EnergyController.h>

#define BUTTONS_COUNT 2

enum ButtonKeys : uint8_t
{
    BUTTON_FRONT = 0,
    BUTTON_BACK
};

enum SwipeType_t : uint8_t
{
    SWIPE_NONE = 0,
    SWIPE_FORWARD,
    SWIPE_BACK
};

struct ButtonStateChangedEventArgs
{
public:
    ButtonKeys Key;
    ButtonAction_t Action;
    ButtonStateChangedEventArgs() = default;
    ButtonStateChangedEventArgs(ButtonKeys key, ButtonAction_t action);
};

class ButtonsController
{
private:
    ButtonsController() = default;
    ButtonsController(const ButtonsController &) = delete;
    ButtonsController &operator=(const ButtonsController &) = delete;

    TouchButton **buttons_ = nullptr;

    static uint32_t swipePressedTime_;
    static int8_t swipeCheckDir_;
    static uint16_t swipeCheckIndex_;
    static SemaphoreHandle_t mutex_;
    static TaskHandle_t buttonTask_;
    static SwipeType_t swipeDir_;
    static void logicCycle(void *args);
    static SwipeType_t checkSwipe();

public:
    Events<SwipeType_t> OnSwipeRegistred;
    Events<ButtonStateChangedEventArgs> OnButtonStateChanged;

    static ButtonsController &getInstance();

    void initialize();
    TouchButton *getButton(ButtonKeys index);
    TouchButton *operator[](const ButtonKeys index);

    SwipeType_t getSlideDirection();
};

extern ButtonsController &buttons;