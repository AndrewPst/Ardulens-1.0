#ifndef __RESOURCE__
#define __RESOURCE__

#include <Arduino.h>
#include <Settings.h>

#define DIR_SIZE 8
#define LANG_COUNT 2

enum Language : uint8_t
{
    RUS = 0,
    ENG = 1
};

class Resource
{
private:
    const char *directionary_[DIR_SIZE][LANG_COUNT]{
        {"Система", "System"},
        {"Отключнено", "Disconnect..."},
        {"Ожидание\nподключения", "Waiting for\nconnection..."},
        {"Шагов", "Steps"},
        {"Датчик", "Sensor"},
        {"Звонок!", "Calling!"},
        {"Нет звоков!", "No calling!"},
        {"Музыка", "Music"}};

    Language currentLang_ = ENG;

    Resource();
    Resource(const Resource &) = delete;
    Resource &operator=(const Resource &) = delete;

    static void OnSettingsUpdate(Settings *settings);

public:
    void initializate();

    static Resource &getInstance();
    Language getCurrentLang();

    void setLang(Language lang);
    const char *getStr(const char *key);
    const char *getStrByIndex(const uint16_t index);

    const char *operator[](const uint16_t index);
};

extern Resource &resource;

#endif