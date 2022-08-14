#pragma once

#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
#include <Page.h>
#include <ArduinoJson.h>

#include <Fonts/FontsRus/FreeSansOblique12.h>
#include <Fonts/FontsRus/FreeSans9.h>

#define NOTES_ACTION_ADD '0'
#define NOTES_ACTION_EDIT '1'
#define NOTES_ACTION_DELETE '2'
#define NOTES_ACTION_SYNC '3'

const static uint8_t NotesIcon[] PROGMEM{
    0x00,
    0x01,
    0x80,
    0x00,
    0x03,
    0x40,
    0x7f,
    0xc6,
    0x20,
    0xc0,
    0x0f,
    0x10,
    0x80,
    0x19,
    0xb0,
    0x80,
    0x30,
    0xe0,
    0x80,
    0x60,
    0xc0,
    0x80,
    0xc1,
    0x80,
    0x81,
    0x83,
    0x00,
    0x83,
    0x06,
    0x40,
    0x86,
    0x0c,
    0x40,
    0x8c,
    0x18,
    0x40,
    0x8e,
    0x30,
    0x40,
    0x8b,
    0x60,
    0x40,
    0x89,
    0xc0,
    0x40,
    0x8f,
    0x80,
    0x40,
    0x8c,
    0x00,
    0x40,
    0x80,
    0x00,
    0x40,
    0xc0,
    0x00,
    0xc0,
    0x7f,
    0xff,
    0x80,
};

class NotesPage : public PageBase
{
    StaticJsonDocument<1536> jsonBuffer;
    bool isSync_ = false;

    class Graphics
    {
    private:
        uint8_t indexOfOpenedNote_ = 0;

    public:
        void onShow()
        {
            drawSelectedNote();
            if (current_->isSync_ == false)
            {
                ble.send(CMDKEY_CMDPAGE, {"notes_p", "1"});
            }
        }

        void OnTick()
        {
        }

        void onButtonsClicked(ButtonAction_t action, ButtonKeys key)
        {
            if (action == B_MULTICLICK && buttons[key]->getClicksCount() == 1)
            {
                if (key == BUTTON_BACK)
                    indexOfOpenedNote_ =
                        indexOfOpenedNote_ <= 0 ? current_->jsonBuffer.size() - 1 : indexOfOpenedNote_ - 1;
                else
                    indexOfOpenedNote_ =
                        indexOfOpenedNote_ >= current_->jsonBuffer.size() - 1 ? 0 : indexOfOpenedNote_ + 1;
                drawSelectedNote();
            }
            else if (buttons[key]->getButtonAction() == B_HOLD)
            {
                ble.send(CMDKEY_CMDPAGE, {"notes_p", "1"});
            }
        }

        void drawSelectedNote()
        {
            char *title1, *text1;
            title1 = (char *)(current_->jsonBuffer[indexOfOpenedNote_]["title"].isNull() ? "No notes" : current_->jsonBuffer[indexOfOpenedNote_]["title"].as<const char *>());
            text1 = (char *)(current_->jsonBuffer[indexOfOpenedNote_]["text"].isNull() ? "Add notes from your phone" : current_->jsonBuffer[indexOfOpenedNote_]["text"].as<const char *>());
            char* title = new char[strlen(title1) + 1];
            display.convertUTF8(title, title1);
            char* text = new char[strlen(text1) + 1];
            display.convertUTF8(text, text1);

            display.fillRect(0, 0, 160, 32, TFT_DARKGREEN);
            display.fillRect(0, 32, 160, 54, TFT_BLACK);
            display.drawBitmap(0, 6, NotesIcon, 20, 20, TFT_YELLOW);
            display.setFontSize(5);
            display.setTextColor(TFT_WHITE);
            display.setTextDatum(TL_DATUM);
            display.drawString(title, 21, 2);
            display.setFontSize(3);
            display.setViewport(DISPLAY_OFFSET + 2, 32, 126, 48);
            display.setCursor(0, 16);
            display.println(text);
            display.resetAllViewport();
            delete title;
            delete text;
        }

        void onHide()
        {
        }
    };

    Graphics *graphics = nullptr;

    static NotesPage *current_;

public:
    NotesPage()
    {
        current_ = this;
        uploadNotesFromFs();
    }

    void uploadNotesFromFs()
    {
        File f = SPIFFS.open("/notes.txt", FILE_READ);
        auto error = deserializeJson(jsonBuffer, f);
        f.close();
        if (error)
        {
            log_d("Parsing notes error: %s", error.c_str());
        }
    }

    void onAction(DataParser *data)
    {
        if (data->getParameter(1)[0] == NOTES_ACTION_DELETE)
        {
            uint16_t index = atoi(data->getParameter(2));
            for (; index < jsonBuffer.size() - 2; index++)
            {
                jsonBuffer[index] = jsonBuffer[index + 1];
            }
            jsonBuffer.remove(index);
        }
        else if (data->getParameter(1)[0] == NOTES_ACTION_ADD)
        {
            jsonBuffer[jsonBuffer.size()]["title"] = data->getParameter(2);
            jsonBuffer[jsonBuffer.size() - 1]["text"] = data->getParameter(3);
        }
        else if (data->getParameter(1)[0] == NOTES_ACTION_EDIT)
        {
            uint16_t index = atoi(data->getParameter(2));
            jsonBuffer[index]["title"] = data->getParameter(3);
            jsonBuffer[index]["text"] = data->getParameter(4);
        }
        else if (data->getParameter(1)[0] == NOTES_ACTION_SYNC)
        {
            isSync_ = true;
            uint8_t removeCount = jsonBuffer.size() - atoi(data->getParameter(2));
            for (uint8_t i = 0; i < removeCount; i++)
            {
                jsonBuffer.remove(jsonBuffer.size() - 1);
            }
        }
        log_d("Size: %d", jsonBuffer.size());
    }

    void onCreate() override
    {
        if (graphics == nullptr)
            graphics = new Graphics();
        graphics->onShow();
    }

    void onTick() override
    {
        if (graphics != nullptr)
            graphics->OnTick();
    }

    void onButtonClicked(ButtonAction_t button, ButtonKeys key) override
    {
        if (graphics != nullptr)
            graphics->onButtonsClicked(button, key);
    }

    void onDestroy() override
    {
        File f = SPIFFS.open("/notes.txt", FILE_WRITE);
        serializeJson(jsonBuffer, f);
        f.close();
        if (graphics != nullptr)
            delete graphics;
        graphics = nullptr;
    }

    inline bool isOpened()
    {
        return graphics != nullptr;
    }
};

NotesPage *NotesPage::current_ = nullptr;

class NotesPageShell : public PageShell
{
public:
    NotesPageShell() : PageShell()
    {
        page_ = GetInstancePage();
    }

    inline PageBase *GetInstancePage() override
    {
        return new NotesPage();
    }
    inline const char *getKey() override
    {
        return "notes_p";
    }

    inline void Dispose() override
    {
        page_->onDestroy();
    }

    inline bool isCreated() override
    {
        return true;
    }

    inline bool isOpened() override
    {
        return ((NotesPage *)page_)->isOpened();
    }
};