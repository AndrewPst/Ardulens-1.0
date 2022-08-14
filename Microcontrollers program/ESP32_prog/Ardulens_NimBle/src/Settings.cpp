#include <Settings.h>

Preferences preferences = Preferences();

SemaphoreHandle_t Settings::mutex_ = xSemaphoreCreateMutex();

#define ENTER xSemaphoreTake(mutex_, portMAX_DELAY)
#define EXIT xSemaphoreGive(mutex_)

Settings &Settings::getInstance()
{
    static Settings s;
    return s;
}

void Settings::OnSetupDataReceived(DataParser *data)
{
    if (data->getLenght() < SETUP_ALL)
    {
        log_i("Settings data not correct!");
        return;
    }
    ENTER;
    for (uint8_t i = 0; i < SETUP_ALL; i++)
    {
        settings.settings_[i] = atoi(data->getParameter(i));
        settings.preferences.putUInt(String(i).c_str(), settings.settings_[i]);
    }
    EXIT;
    settings.OnSettingsUpdated.Invoke(&settings);
}

void Settings::Initialize()
{
    ble.OnNewDataReceived.subscribe(CMDKEY_SETUP, OnSetupDataReceived);
    loadSettings();
}

uint32_t &Settings::getValueByKey(SetupKeys key)
{
    ENTER;
    uint32_t &value = settings_[(uint8_t)key];
    EXIT;
    return value;
}

void Settings::saveSettings()
{
    ENTER;
    preferences.begin("settings");
    for (uint16_t i = 0; i < (uint16_t)SETUP_ALL; i++)
    {
        preferences.putUInt(String(i).c_str(), settings_[i]);
    }
    preferences.end();
    EXIT;
}

void Settings::loadSettings()
{
    ENTER;
    preferences.begin("settings");
    for (uint16_t i = 0; i < (uint16_t)SETUP_ALL; i++)
    {
        settings_[i] = preferences.getUInt(String(i).c_str(), settings_[i]);
    }
    preferences.end();
    EXIT;
}

void Settings::setValue(int8_t value, const char *path)
{
    ENTER;
    preferences.begin("cache");
    preferences.putChar(path, value);
    preferences.end();
    EXIT;
}
int8_t Settings::getValue(const char *path, int8_t defaultValue)
{
    int8_t result;
    ENTER;
    preferences.begin("cache");
    result = preferences.getChar(path, defaultValue);
    preferences.end();
    EXIT;
    return result;
}

void Settings::setValue(int16_t value, const char *path)
{
    ENTER;
    preferences.begin("cache");
    preferences.putShort(path, value);
    preferences.end();
    EXIT;
}
int16_t Settings::getValue(const char *path, int16_t defaultValue)
{
    int16_t result;
    ENTER;
    preferences.begin("cache");
    result = preferences.getShort(path, defaultValue);
    preferences.end();
    EXIT;
    return result;
}

void Settings::setValue(int32_t value, const char *path)
{
    ENTER;
    preferences.begin("cache");
    preferences.putInt(path, value);
    preferences.end();
    EXIT;
}
int32_t Settings::getValue(const char *path, int32_t defaultValue)
{
    int32_t result;
    ENTER;
    preferences.begin("cache");
    result = preferences.getInt(path, defaultValue);
    preferences.end();
    EXIT;
    return result;
}

void Settings::setValue(bool value, const char *path)
{
    ENTER;
    preferences.begin("cache");
    preferences.putBool(path, value);
    preferences.end();
    EXIT;
}
bool Settings::getValue(const char *path, bool defaultValue)
{
    bool result;
    ENTER;
    preferences.begin("cache");
    result = preferences.getBool(path, defaultValue);
    preferences.end();
    EXIT;
    return result;
}

// void Settings::setValue(String& value, const char *path)
// {
//     ENTER;
//     preferences.begin("cache");
//     preferences.putString(path, value);
//     preferences.end();
//     EXIT;
// }
// String Settings::getValue(const char *path, String& defaultValue)
// {
//     String result;
//     ENTER;
//     preferences.begin("cache");
//     result = preferences.getString(path, defaultValue);
//     preferences.end();
//     EXIT;
//     return result;
// }

Settings &settings = Settings::getInstance();