#pragma once

#include <Arduino.h>
#include <Page.h>
#include <Display.h>
#include <DataParser.h>
#include <Settings.h>
#include <Resource.h>

class StepsPage : public PageBase
{
private:
    uint8_t animCounter = 0;
    uint16_t overSteps_ = 0;
    uint32_t updateTimer_ = 0;

    xTaskHandle animTask_;
    static SemaphoreHandle_t displayMutex_;
    static SemaphoreHandle_t animMutex_;

    static bool isAnimLoaded;
    TFT_eSprite **animation = nullptr;

    char* stepsWord_;

public:
    void onCreate() override
    {
        stepsWord_ = display.convertUTF8((char*)resource[3]);

        animation = new TFT_eSprite *[13]();
        isAnimLoaded = false;

        xSemaphoreTake(displayMutex_, portMAX_DELAY);
        display.setFontSize(8);
        display.setTextColor(TFT_DARKCYAN);
        xSemaphoreGive(displayMutex_);
        printStepsCount(motionDetector.getStepsCountPerDay());

        xTaskCreateUniversal(loadAnimation, "loadAnim", 4096, (void *)animation, 1, &animTask_, 2);
    }

    void onTick() override
    {
        if (millis() - updateTimer_ > 100)
        {
            uint16_t steps = motionDetector.getStepsCountPerDay();
            if (steps != overSteps_)
            {
                overSteps_ = steps;
                printStepsCount(steps);
            }
            xSemaphoreTake(animMutex_, portMAX_DELAY);
            if (isAnimLoaded)
            {
                xSemaphoreTake(displayMutex_, portMAX_DELAY);
                animation[animCounter]->pushSprite(0, 4);
                xSemaphoreGive(displayMutex_);
                animCounter++;
                if (animCounter >= 12)
                    animCounter = 0;
            }
            xSemaphoreGive(animMutex_);
            updateTimer_ = millis();
        }
    }

    void printStepsCount(uint16_t steps)
    {
        xSemaphoreTake(displayMutex_, portMAX_DELAY);
        display.fillScreen(TFT_WHITE);
        display.setCursor(2, 78);
        display.setFontSize(8);
        display.print(steps);
        display.print(' ');
        display.setFontSize(5);
        display.print(stepsWord_);
        xSemaphoreGive(displayMutex_);
    }

    void onAction(DataParser *data) override
    {
    }

    void onDestroy() override
    {
        xSemaphoreTake(animMutex_, portMAX_DELAY);
        if (!isAnimLoaded && eTaskGetState(animTask_) != eTaskState::eDeleted)
        {
            vTaskDelete(animTask_);
        }
        for (uint8_t i = 0; i < 13; i++)
        {
            delete animation[i];
        }
        delete[] animation;
        delete[] stepsWord_;
        xSemaphoreGive(animMutex_);
        motionDetector.saveSteps();
    }

    static void loadAnimation(void *param)
    {
        TFT_eSprite **animation = (TFT_eSprite **)param;
        for (uint8_t i = 1; i <= 12; i++)
        {
            xSemaphoreTake(animMutex_, portMAX_DELAY);
            animation[i - 1] = new TFT_eSprite(&display);
            animation[i - 1]->setColorDepth(8);
            animation[i - 1]->createSprite(30, 41);
            //xSemaphoreTake(displayMutex_, portMAX_DELAY);
            display.drawJpeg((String("/runner") + String((uint16_t)i) + String(".jpg")).c_str(), 0, 0, animation[i - 1]);
            //xSemaphoreGive(displayMutex_);
            xSemaphoreGive(animMutex_);
            vTaskDelay(1);
        }
        xSemaphoreTake(animMutex_, portMAX_DELAY);
        isAnimLoaded = true;
        xSemaphoreGive(animMutex_);
        vTaskDelete(nullptr);
    }
};

SemaphoreHandle_t StepsPage::displayMutex_ = xSemaphoreCreateMutex();
SemaphoreHandle_t StepsPage::animMutex_ = xSemaphoreCreateMutex();
bool StepsPage::isAnimLoaded = false;

class StepsPageShell : public PageShell
{
public:
    inline PageBase *GetInstancePage() override
    {
        return new StepsPage();
    }
    inline const char *getKey() override
    {
        return "steps_p";
    }
};