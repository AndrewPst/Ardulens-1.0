#ifndef __SETTINGS__
#define __SETTINGS__

#include <Arduino.h>
#include <DataParser.h>
#include <BleController.h>
#include <Display.h>
#include <EnergyController.h>
#include <Preferences.h>

//Здесь добавляем ключи настроек
enum SetupKeys : uint8_t
{
    SETUP_LANG = 0,
    SETUP_BRIGHTNESS = 1,
    SETUP_RETURN_TO_HOME_PAGE_TIME = 2,
    SETUP_SHUTDOWN_TIME = 3,
    SETUP_ACC_SENSATIVITY = 4,
    SETUP_ALL
};

#define DEFAULT_LANG 0
#define DEFAULT_BRIGHTNESS 200
#define DEFAULT_RETURN_TO_HOME_PAGE_TIME 30
#define DEFAULT_SHUTDOWN_TIME 5
#define DEFAULT_ACC_SENSATIVITY 100

class Settings
{
private:
    Preferences preferences;

    uint32_t *settings_ = new uint32_t[SETUP_ALL]{
        DEFAULT_LANG,
        DEFAULT_BRIGHTNESS,
        DEFAULT_RETURN_TO_HOME_PAGE_TIME,
        DEFAULT_SHUTDOWN_TIME,
        DEFAULT_ACC_SENSATIVITY,
    };

    static SemaphoreHandle_t mutex_;

    static void OnSetupDataReceived(DataParser *data);

    Settings() = default;
    Settings(const Settings &) = delete;
    Settings &operator=(const Settings &) = delete;

    void saveSettings();
    void loadSettings();

public:
    static Settings &getInstance();
    Events<Settings*> OnSettingsUpdated;

    void Initialize();
    uint32_t &getValueByKey(SetupKeys key);

    void setValue(int8_t value, const char *path);
    int8_t getValue(const char *path, int8_t defaultValue);

    void setValue(int16_t value, const char *path);
    int16_t getValue(const char *path, int16_t defaultValue);

    void setValue(int32_t value, const char *path);
    int32_t getValue(const char *path, int32_t defaultValue);

    void setValue(bool value, const char *path);
    bool getValue(const char *path, bool defaultValue);

    // void setValue(String& value, const char *path);
    // String getValue(const char *path, String& defaultValue);
};

extern Settings &settings;

#endif