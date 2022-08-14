#include <Page.h>

QueueHandle_t PageShell::buttonsQueue = xQueueCreate(3, sizeof(ButtonStateChangedEventArgs));

void PageBase::onCreate() {}
void PageBase::onAction(DataParser *data) {}
void PageBase::onButtonClicked(ButtonAction_t button, ButtonKeys key) {}
void PageBase::onMotionDetected(uint8_t motionType) {}
void PageBase::onTick() {}
void PageBase::onDestroy() {}

void PageShell::tick()
{
    page_->onTick();
    ButtonStateChangedEventArgs buffer;
    if (xQueueReceive(buttonsQueue, &buffer, 1) == pdTRUE)
    {
        if (millis() - pageOpeningTime > 1000)
            page_->onButtonClicked(buffer.Action, buffer.Key);
    }
}

bool PageShell::isCreated()
{
    return page_ != nullptr;
}

bool PageShell::isOpened()
{
    return page_ != nullptr;
}

void PageShell::Dispose()
{
    if (page_ == nullptr)
        return;
    log_d("Destroyed: %s", getKey());
    page_->onDestroy();
    delete page_;
    page_ = nullptr;
}

PageBase *PageShell::getPage()
{
    return page_;
}

void PageShell::createPage()
{
    pageOpeningTime = millis();
    if (page_ == nullptr)
        page_ = GetInstancePage();
    page_->onCreate();
    log_d("Created: %s", getKey());
}

void PageShell::init()
{
    buttons.OnButtonStateChanged.subscribe(OnButtonClicked);
}

void PageShell::OnButtonClicked(ButtonStateChangedEventArgs state)
{
    xQueueSend(buttonsQueue, (void *)&state, 5);
}