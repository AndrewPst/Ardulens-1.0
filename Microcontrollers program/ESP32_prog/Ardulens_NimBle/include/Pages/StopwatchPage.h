#pragma once

#include <Arduino.h>
#include <Page.h>
#include <Display.h>
#include <DataParser.h>
#include <EnergyController.h>

class StopwatchPage : public PageBase
{
private:
    TFT_eSprite time_ = TFT_eSprite(&display);

    static uint16_t millisCounter;
    static uint16_t secondsCounter;

    bool isEnabled_ = false;
    uint32_t updateTimer_ = 0;

public:
    void onCreate() override
    {
        display.fillScreen(TFT_BLACK);
        display.drawJpeg("/swatch.jpg", 8, 8);
        display.drawFastHLine(10, 48, 140, TFT_ORANGE);
        time_.setColorDepth(8);
        time_.createSprite(160, 30);
        time_.setTextColor(TFT_WHITE);
        time_.setFreeFont(&FreeSerif12pt7b);
        drawTime();
    }

    void onTick() override
    {
        if (millis() - updateTimer_ >= 1)
        {
            if (isEnabled_)
            {
                energy.updateLastActiveTime();
                drawTime();
                millisCounter += millis() - updateTimer_;
                if (millisCounter >= 1000)
                {
                    millisCounter = 0;
                    secondsCounter++;
                }
            }
            updateTimer_ = millis();
        }
    }

    void drawTime()
    {
        time_.fillSprite(TFT_BLACK);
        time_.setCursor(0, 26);
        time_.print(secondsCounter);
        time_.print('.');
        if (millisCounter < 100)
            time_.print('0');
        if (millisCounter < 10)
            time_.print('0');
        time_.print(millisCounter);
        time_.pushSprite(4, 50);
    }

    void onButtonClicked(ButtonAction_t action, ButtonKeys key) override
    {
        if (action == B_CLICK)
        {
            isEnabled_ = !isEnabled_;
        }
        else if (action == B_HOLDED && !isEnabled_)
        {
            millisCounter = 0;
            secondsCounter = 0;
            drawTime();
        }
    }

    void onAction(DataParser *data) override
    {
    }

    void onDestroy() override
    {
    }
};

uint16_t StopwatchPage::millisCounter = 0;
uint16_t StopwatchPage::secondsCounter = 0;

class StopwatchPageShell : public PageShell
{
public:
    inline PageBase *GetInstancePage() override
    {
        return new StopwatchPage();
    }
    inline const char *getKey() override
    {
        return "swatch_p";
    }
};