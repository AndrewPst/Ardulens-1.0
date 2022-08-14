#pragma once

#include <Arduino.h>
#include <PagesController.h>
#include <Page.h>
#include <Display.h>
#include <DataParser.h>
#include <MotionDetector.h>
#include <Resource.h>

#include <Fonts/FontsRus/FreeSans14.h>
#include <Fonts/FontsRus/FreeSans10.h>

class WeatherPage : public PageBase
{
private:
    float sensorTemp_ = 0, dataTemp_ = 0;
    uint32_t timer_ = 0;
    char *description_ = nullptr;
    bool isSensorModeOpened;

    char* sensorWord_;

public:
    inline void onCreate() override
    {
        sensorWord_ = display.convertUTF8((char*)resource[4]);
        sensorTemp_ = motionDetector.getTemperature();
        showSensorData();
    }

    inline void onTick() override
    {
    }

    inline void onAction(DataParser *data) override
    {
        dataTemp_ = atof(data->getParameter(1));
        description_ = display.convertUTF8(data->getParameter(2));
        showReceivedData();
    }

    inline void onButtonClicked(ButtonAction_t button, ButtonKeys key) override
    {
        if (button == B_HOLD)
        {
            if (description_ == nullptr)
                ble.send(CMDKEY_CMDPAGE, {"weat_p", "1"});
            else if (isSensorModeOpened == true)
                showReceivedData();
            else
                showSensorData();
        }
    }

    inline void showSensorData()
    {
        isSensorModeOpened = true;
        display.fillScreen(TFT_BLACK);
        display.setFontSize(6);
        display.setTextDatum(TL_DATUM);
        display.setTextColor(TFT_LIGHTGREY);
        display.drawString(sensorWord_, 0, 2);
        display.fillRect(0, 36, 160, 30, TFT_DARKCYAN);
        display.fillRoundRect(0, 38, 100, 26, 5, TFT_BLACK);
        display.setCursor(5, 58);
        display.setTextColor(TFT_WHITE);
        display.setFontSize(4);
        display.print(sensorTemp_);
        display.println("*c.");
    }

    inline void showReceivedData()
    {
        isSensorModeOpened = false;
        display.fillScreen(TFT_BLACK);
        display.setFontSize(4);
        display.setTextDatum(TL_DATUM);
        display.setTextColor(TFT_LIGHTGREY);
        display.setCursor(0, 46);
        display.print(description_);
        display.fillRect(0, 0, 160, 30, TFT_DARKCYAN);
        display.fillRoundRect(0, 2, 100, 26, 5, TFT_BLACK);
        display.setCursor(5, 22);
        display.setTextColor(TFT_WHITE);
        display.print(dataTemp_);
        display.println("*c.");
    }

    inline void onDestroy() override
    {
        if (description_ != nullptr)
        {
            delete description_;
            description_ = nullptr;
        }
        delete[] sensorWord_;
    }
};

class WeatherPageShell : public PageShell
{
public:
    inline PageBase *GetInstancePage() override
    {
        return new WeatherPage();
    }
    inline const char *getKey() override
    {
        return "weat_p";
    }
};