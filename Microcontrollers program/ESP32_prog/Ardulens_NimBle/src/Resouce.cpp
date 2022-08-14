#include <Resource.h>

Resource &Resource::getInstance()
{
    static Resource res;
    return res;
}

Resource::Resource()
{
}

void Resource::initializate()
{
    settings.OnSettingsUpdated += OnSettingsUpdate;
    currentLang_ = (Language)settings.getValueByKey(SetupKeys::SETUP_LANG);
}

void Resource::setLang(Language lang)
{
    currentLang_ = lang;
}

Language Resource::getCurrentLang()
{
    return currentLang_;
}

const char *Resource::getStrByIndex(const uint16_t index)
{
    const char* result = directionary_[index][(uint16_t)currentLang_];
    log_d("Index - %d, Lang - %d, Text - %s", index, currentLang_, result);
    return result;
}

const char *Resource::operator[](const uint16_t index)
{
    return getStrByIndex(index);
}

void Resource::OnSettingsUpdate(Settings *settings)
{
    resource.currentLang_ = (Language)settings->getValueByKey(SetupKeys::SETUP_LANG);
    log_d("cur lang: %d", resource.currentLang_);
}

Resource &resource = Resource::getInstance();