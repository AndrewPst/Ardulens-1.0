#pragma once

#pragma once

#include <Arduino.h>
#include <Page.h>
#include <Display.h>
#include <DataParser.h>

//uint32_t readBatteryVoltage();
uint16_t readBatteryPercent();

#define IMAGE1_WIDTH 42
#define IMAGE2_WIDTH 18

class BatteryPage : public PageBase
{

public:
    void onCreate() override
    {
        display.fillScreen(TFT_BLACK);
        display.setFontSize(5);
        display.setTextColor(TFT_WHITE);
        display.setTextDatum(BC_DATUM);
        drawGlassesBatt(readBatteryPercent());
        //drawPhoneBatt(BATT_LEVEL);
        display.drawRoundRect(0, 0, display.width(), 80, 3, TFT_LIGHTGREY);
    }

    void drawGlassesBatt(int16_t level)
    {
        display.setViewport(DISPLAY_OFFSET + 2, 2, display.width()/2-4, 76);
        display.fillScreen(TFT_BLACK);
        display.drawJpeg("/GlassesIconS.jpg", (display.width() - IMAGE1_WIDTH)/2, 2);
        display.setTextColor(level > 20 ? TFT_WHITE : TFT_ORANGE);
        display.drawString((String(level) + '%').c_str(), display.width()/2, 74);
        display.resetAllViewport();
    }

    void drawPhoneBatt(uint16_t level)
    {
        display.setViewport(DISPLAY_OFFSET + display.width()/2 , 2, display.width()/2-4, 76);
        display.fillScreen(TFT_BLACK);
        display.drawJpeg("/PhoneIconS.jpg", (display.width() - IMAGE2_WIDTH)/2, 0);
        display.setTextColor(level > 15 ? TFT_WHITE : TFT_ORANGE);
        display.drawString((String(level) + '%').c_str(), display.width()/2, 74);
        display.resetAllViewport();
    }

    void onTick() override
    {
    }

    void onAction(DataParser *data) override
    {
        uint16_t value = atoi(data->getParameter(1));
        drawPhoneBatt(value);
    }

    void onDestroy() override
    {
    }
};

class BatteryPageShell : public PageShell
{
public:
    inline PageBase *GetInstancePage() override
    {
        return new BatteryPage();
    }
    inline const char *getKey() override
    {
        return "batt_p";
    }
};