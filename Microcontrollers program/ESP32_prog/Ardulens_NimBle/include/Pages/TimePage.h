#pragma once

#include <Arduino.h>
#include <Page.h>
#include <Display.h>
#include <DataParser.h>
//#include <utf8rus.h>

#include <Fonts/FontsRus/Bahamas10.h>

#define X_OFFSET 0
#define Y_OFFSET 10

#define WIDGET_HEIGHT 60
#define WIDGET_WIDTH 114

#define COLOR TFT_GREENYELLOW

#include <TimeData.h>
#include <TimeParser.h>

class TimePage : public PageBase
{
private:
    uint32_t timer1;

public:
    void onCreate() override
    {
        color = (uint32_t)settings.getValue("t_p/color", (int32_t)TFT_SKYBLUE);
        timeNow.updateEpochTime();
        TimeData_t time = timeNow.getTimeData();
        drawWidget(time);
    }

    void onTick() override
    {
        if (millis() - timer1 >= 5000)
        {
            timer1 = millis();
            timeNow.updateEpochTime();
            TimeData_t time = timeNow.getTimeData();
            if (!(time.getValue(T_MINUTE).equals(last_min)))
                drawWidget(time);
        }
        display.setTextColor(display.color565(counter, counter, counter));
        display.drawString(":", X_OFFSET + WIDGET_WIDTH / 2, Y_OFFSET);
        counter += dir;
        if (counter == 255 || counter == 0)
            dir *= -1;
    }

    void onAction(DataParser *data) override
    {
        if (!strcmp(data->getParameter(1), "color"))
        {
            uint32_t last_c = color;
            setColor(atoi(data->getParameter(2)), atoi(data->getParameter(3)), atoi(data->getParameter(4)));
            if (last_c == color)
                return;
            timeNow.updateEpochTime();
            TimeData_t time = timeNow.getTimeData();
            drawWidget(time);
            settings.setValue((int32_t)color, "t_p/color");
        }
    }

    void setColor(uint8_t r, uint8_t g, uint8_t b)
    {
        color = display.color565(r, g, b);
    }

    void drawWidget(TimeData_t &time)
    {
        last_min = time.getValue(T_MINUTE);
        display.fillScreen(TFT_BLACK);

        display.drawRoundRect(X_OFFSET, Y_OFFSET, WIDGET_WIDTH, WIDGET_HEIGHT, 4, color);

        display.setTextDatum(BC_DATUM);
        display.setTextColor(color, TFT_BLACK);
        display.setFreeFont(&Bahamas10pt8b);

        display.drawString(time.getDateStr(true), X_OFFSET + WIDGET_WIDTH / 2, Y_OFFSET + WIDGET_HEIGHT + 5);
        display.drawFastHLine(X_OFFSET, Y_OFFSET + 40, WIDGET_WIDTH, color);

        display.setTextColor(TFT_WHITE);
        display.setFreeFont(&FreeSerif24pt7b);

        display.setTextDatum(TL_DATUM);
        display.drawString(time.getValue(T_HOUR), X_OFFSET + 4, Y_OFFSET + 4);

        display.setTextDatum(TR_DATUM);
        display.drawString(time.getValue(T_MINUTE), X_OFFSET + WIDGET_WIDTH - 4, Y_OFFSET + 4);

        //display.setTextDatum(TC_DATUM);
        //display.drawString(":", X_OFFSET + WIDGET_WIDTH / 2, Y_OFFSET);
    }

    void onDestroy() override
    {
    }
    int8_t dir = 1;
    uint8_t counter = 0;
    String last_min;
    uint32_t color = TFT_GREENYELLOW;
};

class TimePageShell : public PageShell
{
public:
    inline PageBase *GetInstancePage() override
    {
        return new TimePage();
    }
    inline const char *getKey() override
    {
        return "time_p";
    }
};