#pragma once

#include <Arduino.h>
#include <Page.h>
#include <Display.h>
#include <DataParser.h>
#include <Resource.h>
//#include <utf8rus.h>

#include <Fonts/FontsRus/FreeSans12.h>

class PhonePage : public PageBase
{
private:
    uint32_t timer1;
    char *abonentName_;
    bool flag_ = false;
    uint16_t linePos_ = 0;
    uint32_t timer_ = 0;

public:
    void onCreate() override
    {
        pages.setNotificationEnabled(false);
        display.fillScreen(TFT_BLACK);
        display.setTextColor(TFT_WHITE);
        display.setFontSize(5);
        drawEmptyScreen();
        //drawCallScreen();
    }

    void onTick() override
    {
        if (millis() - timer_ >= 30)
        {
            display.drawPixel(linePos_ += 5, 38, !flag_ ? TFT_WHITE : TFT_BLACK);
            if (linePos_ >= display.width())
            {
                linePos_ = 0;
                flag_ = !flag_;
            }
            timer_ = millis();
        }
    }

    void onAction(DataParser *data) override
    {
        display.fillScreen(TFT_BLACK);
        if (data->getParameter(1)[0] == '0')
        {
            drawEmptyScreen();
        }
        else if(data->getParameter(1)[0] == '1')
        {
            abonentName_ = data->getParameter(2);
            drawCallScreen();
        }
    }

    void onButtonClicked(ButtonAction_t button, ButtonKeys key) override
    {
        if (button == B_MULTICLICK && buttons[key]->getClicksCount() > 1)
        {
            ble.send(CMDKEY_CMDPAGE, {"phone_p", key == BUTTON_BACK ? "0" : "1"});
        }
    }
    void drawCallScreen()
    {
        display.drawJpeg("/ResetCall_s.jpg", 0, 0);
        display.drawJpeg("/Mute_small.jpg", display.width()-30, 0);
        display.setTextDatum(TL_DATUM);
        char* abonent = display.convertUTF8(abonentName_);
        display.drawString(abonent, 0, 45);
        delete abonent;
    }

    void drawEmptyScreen()
    {
        char* text = display.convertUTF8((char*)resource[6]);
        display.setTextDatum(TC_DATUM);
        display.drawString(text, display.width()/2, 5);
        display.setTextDatum(TL_DATUM);
        display.drawString(" ---", 0, 45);
        delete[] text;
    }

    void onDestroy() override
    {
        pages.setNotificationEnabled(true);
    }
};

class PhonePageShell : public PageShell
{
public:
    inline PageBase *GetInstancePage() override
    {
        return new PhonePage();
    }
    inline const char *getKey() override
    {
        return "phone_p";
    }
};