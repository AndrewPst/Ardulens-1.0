#include <ButtonsController.h>

#define SLIDE_STATE_NONE 0
#define SLIDE_STATE_BACK 1
#define SLIDE_STATE_FRONT 2
#define SLIDE_STATE_ALL 3

SemaphoreHandle_t ButtonsController::mutex_ = xSemaphoreCreateMutex();
TaskHandle_t ButtonsController::buttonTask_;
SwipeType_t ButtonsController::swipeDir_ = SWIPE_NONE;
uint32_t ButtonsController::swipePressedTime_ = 0;
int8_t ButtonsController::swipeCheckDir_ = 0;
uint16_t ButtonsController::swipeCheckIndex_ = 0;

ButtonStateChangedEventArgs::ButtonStateChangedEventArgs(ButtonKeys key, ButtonAction_t action)
{
    Key = key;
    Action = action;
}

ButtonsController &ButtonsController::getInstance()
{
    static ButtonsController bc;
    return bc;
}

void ButtonsController::initialize()
{
    buttons_ = new TouchButton *[BUTTONS_COUNT]
    {
        new TouchButton(33),
            new TouchButton(32),
    };

    xTaskCreateUniversal(logicCycle, "but_control", 4196, nullptr, 1, &buttonTask_, 1);
}

void ButtonsController::logicCycle(void *args)
{
    for (;;)
    {
        for (uint8_t i = 0; i < BUTTONS_COUNT; i++)
        {
            buttons.buttons_[i]->tick();
            
            if (buttons.buttons_[i]->isNewAction())
                buttons.OnButtonStateChanged.Invoke(ButtonStateChangedEventArgs((ButtonKeys)i, buttons.buttons_[i]->getButtonAction()));

            if (buttons[BUTTON_FRONT]->isStateChanged() || buttons[BUTTON_BACK]->isStateChanged())
                energy.updateLastActiveTime();
        }

        auto swipe = checkSwipe();
        if (swipe != SWIPE_NONE)
        {
            log_d("swipe: %d", (uint16_t)swipe);
            buttons.OnSwipeRegistred.Invoke(swipe);
        }
        delay(5);
    }
    vTaskDelete(nullptr);
}

SwipeType_t ButtonsController::checkSwipe()
{
    SwipeType_t result = SWIPE_NONE;
    if (swipeCheckDir_ == 0) // если сейчас свайп не происходит
    {
        //если нажата первая или последняя кнопка
        if (buttons.buttons_[0]->isStateChanged() &&
            buttons.buttons_[0]->isPressed())
        {
            swipeCheckDir_ = +1;
            swipePressedTime_ = millis();
        }
        else if (buttons.buttons_[BUTTONS_COUNT - 1]->isStateChanged() &&
                 buttons.buttons_[BUTTONS_COUNT - 1]->isPressed())
        {
            swipeCheckDir_ = -1;
            swipePressedTime_ = millis();
        }
        //вычисляем направление проверки свайпа (вперед или назад)
        swipeCheckIndex_ = (swipeCheckDir_ == -1) ? (BUTTONS_COUNT - 2) : 1;
    }
    //если кнопка отпущена до нажатия следующей, то обработка свайпа останавливается
    else if (buttons.buttons_[swipeCheckIndex_ - swipeCheckDir_]->isStateChanged() &&
             !buttons.buttons_[swipeCheckIndex_ - swipeCheckDir_]->isPressed())
    {
        if (!buttons.buttons_[swipeCheckIndex_]->isPressed())
            swipeCheckDir_ = 0;
    }
    // если нет, то проверяем нажатие следующей кнопки
    else
    {
        //если следующая нажата
        if (buttons.buttons_[swipeCheckIndex_]->isStateChanged() &&
            buttons.buttons_[swipeCheckIndex_]->isPressed())
        {
            //если время не слишком маленькое или большое
            uint32_t dif = millis() - swipePressedTime_;
            if (dif > 40 && dif < 300)
            {
                //регистрируем нажатие следующей кнопки
                swipePressedTime_ = millis();
                //если нажата последняя, то вычисляем направление
                if (swipeCheckIndex_ == 0 || swipeCheckIndex_ == BUTTONS_COUNT - 1)
                {
                    result = swipeCheckDir_ == -1 ? SWIPE_BACK : SWIPE_FORWARD;
                    swipeCheckDir_ = 0;
                }
                //если нет, то движемся дальше по направлению
                else
                    swipeCheckIndex_ += swipeCheckDir_;
            }
            //если в отведенное время не уместилось, то прерываем обработку
            else
                swipeCheckDir_ = 0;
        }
    }
    return result;
}

TouchButton *ButtonsController::getButton(ButtonKeys index)
{
    xSemaphoreTake(mutex_, portMAX_DELAY);
    TouchButton *butt = buttons_[(uint8_t)index];
    xSemaphoreGive(mutex_);
    return butt;
}

TouchButton *ButtonsController::operator[](ButtonKeys index)
{
    return getButton(index);
}

ButtonsController &buttons = ButtonsController::getInstance();