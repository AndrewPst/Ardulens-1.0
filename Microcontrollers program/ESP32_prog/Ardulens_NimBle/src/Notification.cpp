#include <Notification.h>

#include <Fonts/FontsRus/FreeSansOblique14.h>
#include <Fonts/FontsRus/FreeSans14.h>
#include <Fonts/FontsRus/FreeSans9.h>

NotificationParameters_t::NotificationParameters_t(const char *name, const char *title, const char *text)
{
    Name = (char *)name;
    Text = (char *)text;
    Title = (char *)title;
}

Notification::Notification(NotificationParameters_t *param)
{
    param_ = param;
}
void Notification::showNotifyName()
{
    showTime_ = millis();
    display.fillScreen(TFT_BLACK);
    sprite = new TFT_eSprite(&display);
    sprite->setColorDepth(8);
    sprite->createSprite(39, 32);
    sprite->fillSprite(TFT_BLACK);
    display.drawJpeg("/NewNotify.jpg", 2, 2, sprite);
    sprite->pushSprite(0, 0);
    display.drawRoundRect(0, 35, display.width() + 10, display.height() - 36, 10, TFT_DARKCYAN);
    display.setTextDatum(TL_DATUM);
    display.setTextColor(TFT_SKYBLUE);
    display.setFontSize(6);
    char *name = display.convertUTF8(param_->Name);
    display.drawString(name, 2, 42);
    delete name;
}

void Notification::tick()
{
    if ((millis() & 0b11111) == 0 && !isAllOpened_)
    {
        sprite->pushSprite(counterX += xDir, counterY);
        if (counterX >= 20 || counterX <= 0)
            xDir = -xDir;
    }
}

void Notification::showAllNotify()
{
    //delete sprite;
    showTime_ = millis();
    isAllOpened_ = true;
    display.fillScreen(TFT_BLACK);
    display.fillRoundRect(0, 0, display.width(), display.height(), 6, display.color565(50, 90, 130));
    display.setFontSize(6);
    display.setTextColor(TFT_LIGHTGREY);
    char *text = display.convertUTF8(param_->Title);
    display.drawString(text, 2, -2);
    delete text;
    display.setFontSize(3);
    display.fillRoundRect(-5, 30, display.width(), display.height() - 30, 5, TFT_BLACK);
    display.setViewport(DISPLAY_OFFSET + 4, 32, display.width() - 10, display.height() - 32);
    display.setCursor(0, 18);
    display.setTextColor(TFT_WHITE);
    text = display.convertUTF8(param_->Text);
    display.print(text);
    delete text;
    display.resetAllViewport();
}

Notification::~Notification()
{
    delete sprite;
}

bool Notification::isAllOpened()
{
    return isAllOpened_;
}

uint32_t &Notification::getShowTime()
{
    return showTime_;
}