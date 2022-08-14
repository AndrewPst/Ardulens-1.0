#pragma once

#include <Arduino.h>
#include <PagesController.h>
#include <Page.h>
#include <Display.h>
#include <DataParser.h>
#include <Resource.h>

#include <Fonts/FontsRus/FreeSerif12.h>

static const uint8_t MusicNextIcon[] PROGMEM{
    0x00,
    0x03,
    0x80,
    0x18,
    0x00,
    0x07,
    0x80,
    0x7c,
    0x00,
    0x1f,
    0x80,
    0xfc,
    0x00,
    0x3f,
    0x81,
    0xfc,
    0x00,
    0xff,
    0x83,
    0xfc,
    0x01,
    0xff,
    0x87,
    0xfc,
    0x07,
    0xff,
    0x8f,
    0xfc,
    0x0f,
    0xff,
    0xff,
    0xfc,
    0x3f,
    0xff,
    0xff,
    0xfc,
    0x7f,
    0xff,
    0xff,
    0xfc,
    0xff,
    0xff,
    0xff,
    0xfc,
    0xff,
    0xff,
    0xff,
    0xfc,
    0x7f,
    0xff,
    0xff,
    0xfc,
    0x3f,
    0xff,
    0xff,
    0xfc,
    0x0f,
    0xff,
    0xff,
    0xfc,
    0x07,
    0xff,
    0x8f,
    0xfc,
    0x01,
    0xff,
    0x87,
    0xfc,
    0x00,
    0xff,
    0x83,
    0xfc,
    0x00,
    0x3f,
    0x81,
    0xfc,
    0x00,
    0x1f,
    0x80,
    0xfc,
    0x00,
    0x0f,
    0x80,
    0x7c,
    0x00,
    0x03,
    0x80,
    0x18,
};

static const uint8_t MusicPreviousIcon[] PROGMEM{
    0x60,
    0x07,
    0x00,
    0x00,
    0xf8,
    0x07,
    0x80,
    0x00,
    0xfc,
    0x07,
    0xe0,
    0x00,
    0xfe,
    0x07,
    0xf0,
    0x00,
    0xff,
    0x07,
    0xfc,
    0x00,
    0xff,
    0x87,
    0xfe,
    0x00,
    0xff,
    0xc7,
    0xff,
    0x80,
    0xff,
    0xff,
    0xff,
    0xc0,
    0xff,
    0xff,
    0xff,
    0xf0,
    0xff,
    0xff,
    0xff,
    0xf8,
    0xff,
    0xff,
    0xff,
    0xfc,
    0xff,
    0xff,
    0xff,
    0xfc,
    0xff,
    0xff,
    0xff,
    0xf8,
    0xff,
    0xff,
    0xff,
    0xf0,
    0xff,
    0xff,
    0xff,
    0xc0,
    0xff,
    0xc7,
    0xff,
    0x80,
    0xff,
    0x87,
    0xfe,
    0x00,
    0xff,
    0x07,
    0xfc,
    0x00,
    0xfe,
    0x07,
    0xf0,
    0x00,
    0xfc,
    0x07,
    0xe0,
    0x00,
    0xf8,
    0x07,
    0xc0,
    0x00,
    0x60,
    0x07,
    0x00,
    0x00,
};

static const uint8_t MusicPauseIcon[] PROGMEM{
    0x00,
    0x03,
    0xfc,
    0x00,
    0x00,
    0x00,
    0x1f,
    0xff,
    0x80,
    0x00,
    0x00,
    0x7f,
    0xff,
    0xe0,
    0x00,
    0x01,
    0xff,
    0xff,
    0xf0,
    0x00,
    0x03,
    0xff,
    0xff,
    0xfc,
    0x00,
    0x07,
    0xff,
    0x0f,
    0xfe,
    0x00,
    0x0f,
    0xf8,
    0x01,
    0xff,
    0x00,
    0x1f,
    0xe0,
    0x00,
    0x7f,
    0x00,
    0x1f,
    0x80,
    0x00,
    0x3f,
    0x80,
    0x3f,
    0x00,
    0x00,
    0x1f,
    0xc0,
    0x3f,
    0x00,
    0x00,
    0x0f,
    0xc0,
    0x7e,
    0x3f,
    0x9f,
    0xc7,
    0xe0,
    0x7c,
    0x3f,
    0x1f,
    0xc7,
    0xe0,
    0xfc,
    0x3f,
    0x1f,
    0x83,
    0xe0,
    0xfc,
    0x3f,
    0x1f,
    0x83,
    0xf0,
    0xf8,
    0x3f,
    0x1f,
    0x81,
    0xf0,
    0xf8,
    0x3f,
    0x1f,
    0x81,
    0xf0,
    0xf8,
    0x3f,
    0x1f,
    0x81,
    0xf0,
    0xf8,
    0x3f,
    0x1f,
    0x81,
    0xf0,
    0xf8,
    0x3f,
    0x1f,
    0x81,
    0xf0,
    0xf8,
    0x3f,
    0x1f,
    0x81,
    0xf0,
    0xfc,
    0x3f,
    0x1f,
    0x83,
    0xf0,
    0xfc,
    0x3f,
    0x1f,
    0x83,
    0xe0,
    0x7c,
    0x3f,
    0x1f,
    0x83,
    0xe0,
    0x7e,
    0x3f,
    0x9f,
    0xc7,
    0xe0,
    0x3f,
    0x1f,
    0x0f,
    0x8f,
    0xc0,
    0x3f,
    0x00,
    0x00,
    0x0f,
    0xc0,
    0x1f,
    0x80,
    0x00,
    0x1f,
    0x80,
    0x1f,
    0xe0,
    0x00,
    0x7f,
    0x80,
    0x0f,
    0xf0,
    0x00,
    0xff,
    0x00,
    0x07,
    0xfe,
    0x07,
    0xfe,
    0x00,
    0x03,
    0xff,
    0xff,
    0xfc,
    0x00,
    0x01,
    0xff,
    0xff,
    0xf8,
    0x00,
    0x00,
    0x7f,
    0xff,
    0xe0,
    0x00,
    0x00,
    0x1f,
    0xff,
    0x80,
    0x00,
    0x00,
    0x03,
    0xfc,
    0x00,
    0x00,
};

static const uint8_t MusicResumeIcon[] PROGMEM{
    0x00,
    0x03,
    0xfc,
    0x00,
    0x00,
    0x00,
    0x1f,
    0xff,
    0x80,
    0x00,
    0x00,
    0x7f,
    0xff,
    0xe0,
    0x00,
    0x01,
    0xff,
    0xff,
    0xf8,
    0x00,
    0x03,
    0xfc,
    0x03,
    0xfc,
    0x00,
    0x07,
    0xe0,
    0x00,
    0xfe,
    0x00,
    0x0f,
    0xc0,
    0x00,
    0x3f,
    0x00,
    0x1f,
    0x9c,
    0x00,
    0x1f,
    0x80,
    0x1f,
    0x3e,
    0x00,
    0x0f,
    0x80,
    0x3e,
    0x3f,
    0x80,
    0x07,
    0xc0,
    0x3c,
    0x3f,
    0xe0,
    0x03,
    0xc0,
    0x7c,
    0x3f,
    0xf8,
    0x01,
    0xe0,
    0x78,
    0x3f,
    0xfc,
    0x01,
    0xe0,
    0x78,
    0x3f,
    0xff,
    0x01,
    0xf0,
    0xf0,
    0x3f,
    0xff,
    0xc0,
    0xf0,
    0xf0,
    0x3f,
    0xff,
    0xf0,
    0xf0,
    0xf0,
    0x3f,
    0xff,
    0xf8,
    0xf0,
    0xf0,
    0x3f,
    0xff,
    0xfc,
    0xf0,
    0xf0,
    0x3f,
    0xff,
    0xfe,
    0xf0,
    0xf0,
    0x3f,
    0xff,
    0xfc,
    0xf0,
    0xf0,
    0x3f,
    0xff,
    0xf0,
    0xf0,
    0xf0,
    0x3f,
    0xff,
    0xc0,
    0xf0,
    0x78,
    0x3f,
    0xff,
    0x01,
    0xf0,
    0x78,
    0x3f,
    0xfe,
    0x01,
    0xe0,
    0x78,
    0x3f,
    0xf8,
    0x01,
    0xe0,
    0x3c,
    0x3f,
    0xe0,
    0x03,
    0xc0,
    0x3e,
    0x3f,
    0x80,
    0x07,
    0xc0,
    0x1f,
    0x3f,
    0x00,
    0x07,
    0x80,
    0x1f,
    0x9c,
    0x00,
    0x0f,
    0x80,
    0x0f,
    0xc0,
    0x00,
    0x3f,
    0x00,
    0x07,
    0xe0,
    0x00,
    0x7e,
    0x00,
    0x03,
    0xfc,
    0x03,
    0xfc,
    0x00,
    0x01,
    0xff,
    0xff,
    0xf8,
    0x00,
    0x00,
    0x7f,
    0xff,
    0xe0,
    0x00,
    0x00,
    0x1f,
    0xff,
    0x80,
    0x00,
    0x00,
    0x07,
    0xfe,
    0x00,
    0x00,
};

static const uint8_t MusicVolumeIcon[] PROGMEM{
    0x00,
    0x03,
    0x00,
    0x00,
    0x00,
    0x07,
    0x80,
    0x00,
    0x00,
    0x07,
    0x80,
    0x00,
    0x00,
    0x0f,
    0x80,
    0x00,
    0x00,
    0x1f,
    0x80,
    0x00,
    0x00,
    0x3f,
    0x80,
    0xe0,
    0x00,
    0x3f,
    0x81,
    0xe0,
    0x00,
    0x7f,
    0xb9,
    0xf0,
    0x00,
    0xff,
    0xbc,
    0xf0,
    0x01,
    0xff,
    0xbe,
    0x78,
    0x71,
    0xff,
    0x9e,
    0x78,
    0xfb,
    0xff,
    0x8f,
    0x3c,
    0xfb,
    0xff,
    0x8f,
    0x3c,
    0xfb,
    0xff,
    0x8f,
    0x3c,
    0xfb,
    0xff,
    0x87,
    0x3c,
    0xf9,
    0xff,
    0x87,
    0x3c,
    0xfb,
    0xff,
    0x87,
    0x3c,
    0xfb,
    0xff,
    0x8f,
    0x3c,
    0xfb,
    0xff,
    0x8f,
    0x3c,
    0x7b,
    0xff,
    0x9e,
    0x78,
    0x01,
    0xff,
    0xbe,
    0x78,
    0x00,
    0xff,
    0xbc,
    0xf8,
    0x00,
    0x7f,
    0xbc,
    0xf0,
    0x00,
    0x7f,
    0x91,
    0xe0,
    0x00,
    0x3f,
    0x81,
    0xe0,
    0x00,
    0x1f,
    0x80,
    0x00,
    0x00,
    0x0f,
    0x80,
    0x00,
    0x00,
    0x0f,
    0x80,
    0x00,
    0x00,
    0x07,
    0x80,
    0x00,
    0x00,
    0x03,
    0x00,
    0x00,
};

#define PAUSE_RESUME_KEY "1"
#define PREVIOUS_KEY "2"
#define NEXT_KEY "3"
#define UP_VOLUME_KEY "4"
#define DOWN_VOLUME_KEY "5"

class MusicPage : public PageBase
{
private:
    bool isVolumeMode_ = false;
    bool isMusicPlaying_ = false;
    uint8_t volume_ = 0;
    uint8_t *animCounter = nullptr;
    int8_t *dir = nullptr;
    TFT_eSprite *anim;
    uint32_t timer = 0;

    char* musicWord_;

public:
    void onCreate() override
    {
        musicWord_ = display.convertUTF8((char*)resource[7]);

        animCounter = new uint8_t[3]{0};
        dir = new int8_t[3]{1, 3, 4};

        anim = new TFT_eSprite(&display);
        anim->setColorDepth(8);
        anim->createSprite(18, 30);
        pages.setNotificationEnabled(false);
        updateScreen();
    }

    void onTick() override
    {
        if (!isVolumeMode_)
        {
            if (millis() - timer > 30)
            {
                anim->fillSprite(TFT_RED);
                for (uint8_t i = 0; i < 3; i++)
                {
                    anim->fillRect(i * 6, animCounter[i], 4, 30 - animCounter[i], TFT_WHITE);
                    animCounter[i] += dir[i];
                    if (animCounter[i] <= 0 || animCounter[i] >= 20)
                        dir[i] = -dir[i];
                }
                anim->pushSprite(88, 10);
                timer = millis();
            }
        }
    }

    void onAction(DataParser *data) override
    {
        bool isMusicPlaying = data->getParameter(1)[0] == '1';
        uint8_t volume = constrain(atoi(data->getParameter(2)), 0, 100);
        if (volume == volume_ && isMusicPlaying_ == isMusicPlaying)
            return;
        isMusicPlaying_ = isMusicPlaying;
        volume_ = volume;
        updateScreen();
    }

    void onButtonClicked(ButtonAction_t button, ButtonKeys key) override
    {
        char *text = nullptr;
        if (button == B_MULTICLICK && !isVolumeMode_)
        {
            if (buttons[key]->getClicksCount() == 1)
                text = (char *)PAUSE_RESUME_KEY;
            else
                text = (char *)((key == BUTTON_BACK) ? PREVIOUS_KEY : NEXT_KEY);
        }
        else if (button == B_HOLDED)
        {
            isVolumeMode_ = !isVolumeMode_;
            updateScreen();
        }
        else if (button == B_CLICK && isVolumeMode_)
        {
            text = (char *)((key == BUTTON_BACK) ? DOWN_VOLUME_KEY : UP_VOLUME_KEY);
        }
        if (text != nullptr)
            ble.send(CMDKEY_CMDPAGE, {"music_p", text});
    }

    void updateScreen()
    {
        display.fillScreen(TFT_RED);
        if (isVolumeMode_)
            drawVolumeMenu();
        else
            drawPlayerMenu();
    }

    void drawPlayerMenu()
    {
        display.setTextDatum(TL_DATUM);
        display.setTextColor(TFT_WHITE);
        display.setFontSize(5);
        display.drawString(musicWord_, 2, 14);
        display.drawBitmap(6, 48, MusicNextIcon, 30, 22, TFT_WHITE, TFT_RED);
        display.drawBitmap(42, 40, !isMusicPlaying_ ? MusicResumeIcon : MusicPauseIcon, 36, 36, TFT_WHITE, TFT_RED);
        display.drawBitmap(84, 48, MusicPreviousIcon, 30, 22, TFT_WHITE, TFT_RED);
    }

    void drawVolumeMenu()
    {
        display.drawBitmap(10, 10, MusicVolumeIcon, 30, 30, TFT_WHITE, TFT_RED);
        display.fillRoundRect(10, 48, volume_, 5, 3, TFT_WHITE);
        display.setTextDatum(TL_DATUM);
        display.setTextColor(TFT_WHITE);
        display.setFontSize(5);
        display.drawString((String(volume_) + '%').c_str(), 45, 16);
        display.fillCircle(10 + volume_, 50, 6, volume_ < 60 ? TFT_GREEN : TFT_ORANGE);
    }

    void onDestroy() override
    {
        pages.setNotificationEnabled(true);
        delete anim;
        delete dir;
        delete[] musicWord_;
    }

    void drawPreviousMusicButton() {}
    void pauseResumeMusicButton() {}
    void nextMusicButton() {}
};

class MusicPageShell : public PageShell
{
public:
    PageBase *GetInstancePage() override
    {
        return new MusicPage();
    }
    const char *getKey() override
    {
        return "music_p";
    }
};