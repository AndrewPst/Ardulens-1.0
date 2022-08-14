#pragma once
#include <Arduino.h>

//#include <utf8rus.h>
#include <Display.h>
#include <JPEGDecoder.h>

struct NotificationParameters_t
{
public:
    char *Name;
    char *Title;
    char *Text;
    NotificationParameters_t(const char *name, const char *title, const char *text);
};

class Notification
{
private:
    NotificationParameters_t *param_;
    uint32_t showTime_ = 0;
    bool isAllOpened_ = false;
    TFT_eSprite* sprite = nullptr;
    uint16_t counterX = 0;
    int8_t counterY = 0, yDir = 1, xDir = 1;
public:
    Notification(NotificationParameters_t *param);
    ~Notification();
    void showNotifyName();
    void showAllNotify();
    void tick();
    bool isAllOpened();

    uint32_t &getShowTime();
};