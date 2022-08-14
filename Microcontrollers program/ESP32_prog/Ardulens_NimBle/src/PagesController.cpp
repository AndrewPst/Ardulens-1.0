#include <PagesController.h>

#include <Pages/MusicPage.h>
#include <Pages/TimePage.h>
#include <Pages/PhonePage.h>
#include <Pages/NotesPage.h>
#include <Pages/WeatherPage.h>
#include <Pages/StepsPage.h>
#include <Pages/BatteryPage.h>
#include <Pages/StopwatchPage.h>

#define ENTER_PAGES xSemaphoreTake(pages.pagesMutex_, portMAX_DELAY)
#define EXIT_PAGES xSemaphoreGive(pages.pagesMutex_)

#define ENTER_NOTIFY xSemaphoreTake(pages.notifyMutex_, portMAX_DELAY)
#define EXIT_NOTIFY xSemaphoreGive(pages.notifyMutex_)

PagesController &PagesController::getInstance()
{
    static PagesController display;
    return display;
}

PagesController::PagesController()
{
    queue_ = xQueueCreate(5, sizeof(PageAction));
    notifyMutex_ = xSemaphoreCreateMutex();
    pagesMutex_ = xSemaphoreCreateMutex();
}

void PagesController::Initialize()
{
    //регистрация событий

    ble.OnNewDataReceived.subscribe(CMDKEY_CMDPAGE, OnUpdatePageCommandReceived);
    ble.OnNewDataReceived.subscribe(CMDKEY_OPEN_PAGE, OnOpenPageCommandReceived);
    ble.OnNewDataReceived.subscribe(CMDKEY_CREATE_NOTIFY, OnCommandCreateNotifyRegistred);

    buttons.OnSwipeRegistred += OnSwipeRegistered;
    energy.onEnergySavingStateChanged += OnEnergyStateChanged;
    settings.OnSettingsUpdated += [](Settings *settings)
    {
        if (!energy.getPowerSaveMode())
            display.setBrightness(settings->getValueByKey(SETUP_BRIGHTNESS));
    };

    //инициализация дисплея
    display.init(INITR_GREENTAB160x80);
    display.setupLedPin();
    display.setRotation(4); //1 & 3 - горизонтально, 4 - зеркально
    display.fillScreen(TFT_BLACK);
    display.resetAllViewport();
    initPages();
    display.setBrightness(settings.getValueByKey(SETUP_BRIGHTNESS)); 

    //создание потока страничного интерфейса
    xTaskCreateUniversal(pageCycle,
                         "page_task",
                         8192, /*(void *)&display*/ nullptr,
                         1, &pageCycleTaskHundle_, 1);
}

void PagesController::initPages()
{
    //вспоминаем индекс домашней страницы
    homePageIndex_ = (uint16_t)settings.getValue("p_home", (int16_t)0);

    ENTER_PAGES;
    PageShell::init();
    pages_.add(new TimePageShell()); //добавляем страницы
    pages_.add(new MusicPageShell());
    pages_.add(new PhonePageShell());
    pages_.add(new NotesPageShell());
    pages_.add(new WeatherPageShell());
    pages_.add(new StepsPageShell());
    pages_.add(new BatteryPageShell());
    pages_.add(new StopwatchPageShell());
    pages_.setCursor(homePageIndex_);
    openedPage_ = pages_.getCursorItem();
    openedPage_->createPage();
    EXIT_PAGES;
}

void PagesController::OnSwipeRegistered(SwipeType_t swipe)
{
    pages.action_ = (swipe == SWIPE_BACK) ? PAGE_PREVIOUS : PAGE_NEXT;
    xQueueSend(pages.queue_, &pages.action_, portMAX_DELAY);
}

void PagesController::OnCommandCreateNotifyRegistred(DataParser *data)
{
    NotificationParameters_t *param = new NotificationParameters_t(data->getParameter(0),
                                                                   data->getParameter(1),
                                                                   data->getParameter(2));
    pages.createNotification(param);
}

void PagesController::OnOpenPageCommandReceived(DataParser *data)
{
    //Проверяем на установку новой домашней страницы
    DataParser *receivedData = ble.getReceivedData();
    if (!strcmp(receivedData->getParameter(0), "sethome"))
    {
        ENTER_PAGES;
        uint16_t index = 0;
        auto page = getPageByKey(data->getParameter(0), &index);
        if (page != nullptr)
        {
            pages.homePageIndex_ = index;
            settings.setValue((int16_t)pages.homePageIndex_, "p_home");
        }
        EXIT_PAGES;
    }
    else
    {
        pages.action_ = PAGE_NEW;
        xQueueSend(pages.queue_, &pages.action_, portMAX_DELAY);
    }
}

void PagesController::OnUpdatePageCommandReceived(DataParser *data)
{
    pages.action_ = PAGE_UPDATE;
    xQueueSend(pages.queue_, &pages.action_, portMAX_DELAY);
    energy.updateLastActiveTime();
}

void PagesController::OpenHomePage()
{
    pages.action_ = PAGE_HOME;
    xQueueSend(pages.queue_, &pages.action_, portMAX_DELAY);
}

void PagesController::OpenNextPage()
{
    pages.action_ = PAGE_NEXT;
    xQueueSend(pages.queue_, &pages.action_, portMAX_DELAY);
}

void PagesController::OpenPreviousPage()
{
    pages.action_ = PAGE_PREVIOUS;
    xQueueSend(pages.queue_, &pages.action_, portMAX_DELAY);
}

void PagesController::OnEnergyStateChanged(bool mode)
{
    pages.action_ = mode ? PAGE_SLEEP : PAGE_WAKE_UP;
    xQueueSend(pages.queue_, &pages.action_, portMAX_DELAY);
}

void PagesController::createNotification(NotificationParameters_t *param)
{
    ENTER_NOTIFY;
    if (notify_ == nullptr && isNotificationEnabled_)
    {
        notifyParams_ = param;
        action_ = PAGE_CREATE_NOTIFY;
        xQueueSend(queue_, &action_, portMAX_DELAY);
    }
    EXIT_NOTIFY;
}

void PagesController::pageCycle(void *param)
{
    PageAction action;
    for (;;)
    {
        // static uint32_t timer = 0;
        // static uint8_t rotation = 0;
        // if(millis() - timer > 5000){
        //     timer = millis();
        //     rotation = rotation >= 8 ? 0 : rotation+1;
        //     display.setRotation(rotation);
        //     log_d("%d", rotation);
        //     PerformAction(PageAction::PAGE_WAKE_UP, nullptr);
        // }
        if (xQueueReceive(pages.queue_, &action, 1) == pdPASS) //если появились действия со страницами
        {
            PerformAction(action, ble.getReceivedData());
        }
        refreshPageTime();
        updateNotify();
    }
}

void PagesController::updateNotify()
{
    bool closeNotify = false;
    ENTER_NOTIFY;
    if (pages.notify_ != nullptr) //если уведомление создано
    {
        pages.notify_->tick(); //тик страницы

        //true если произошло нажатие
        bool isAction = (buttons[BUTTON_BACK]->isNewAction() && buttons[BUTTON_BACK]->getButtonAction() == B_CLICK) || (buttons[BUTTON_FRONT]->isNewAction() && buttons[BUTTON_FRONT]->getButtonAction() == B_CLICK);

        //если зажаты обе кнопки выходим
        if (buttons[BUTTON_BACK]->getButtonAction() == B_PRESSED && buttons[BUTTON_FRONT]->getButtonAction() == B_PRESSED)
            closeNotify = true;
        //если произошло нажатие при развернутом уведомлении
        if (pages.notify_->isAllOpened() && isAction && millis() - pages.notify_->getShowTime() > 500)
        {
            closeNotify = true;
        }
        else if (!pages.notify_->isAllOpened())
        {
            //если уведомление скрыто более 3 секунд - закрывает
            if (millis() - pages.notify_->getShowTime() > 3000)
            {
                closeNotify = true;
            }
            //если нажатие - разворачиваем уведомление
            else if (isAction)
            {
                display.setBrightness(0);
                pages.notify_->showAllNotify();
                display.setBrightness(settings.getValueByKey(SETUP_BRIGHTNESS));
                energy.updateLastActiveTime();
            }
        }
    }
    EXIT_NOTIFY;
    if (closeNotify)
    {
        deleteOpenedNotify();
        PerformAction(PAGE_WAKE_UP, nullptr);
    }
}

void PagesController::deleteOpenedNotify()
{
    ENTER_NOTIFY;
    delete pages.notify_;
    delete pages.notifyParams_;
    pages.notify_ = nullptr;
    EXIT_NOTIFY;
}

void PagesController::refreshPageTime()
{
    ENTER_PAGES;
    if (pages.openedPage_->isCreated() && pages.openedPage_->isOpened()) //делаем тик страницы
    {
        pages.openedPage_->tick();
    }
    //проверяем на необходимость возврата на домашнюю страницу
    else if (settings.getValueByKey(SETUP_RETURN_TO_HOME_PAGE_TIME) > 0)
    {
        if ((millis() - energy.getLastActiveTime()) / 1000 > settings.getValueByKey(SETUP_RETURN_TO_HOME_PAGE_TIME))
        {
            pages.pages_.setCursor(pages.homePageIndex_);
            pages.openedPage_ = pages.pages_.getCursorItem();
        }
    }
    EXIT_PAGES;
}

void PagesController::PerformAction(PageAction action, DataParser *data)
{
    ENTER_PAGES;
    switch (action)
    {
    case PAGE_HOME:
        if (pages.openedPage_ == pages.pages_.get(pages.homePageIndex_))
            break;
        pages.pages_.setCursor(pages.homePageIndex_);
        createSelectedPage();
        break;

    case PAGE_NEXT:
        pages.pages_.moveCursorNext();
        createSelectedPage();
        break;

    case PAGE_PREVIOUS:
        pages.pages_.moveCursorPrevious();
        createSelectedPage();
        break;

    case PAGE_NEW:
    {
        PageShell *newPage = getPageByKey(data->getParameter(0));
        if (newPage != nullptr)
        {
            if (pages.notify_ != nullptr)
                deleteOpenedNotify();
            pages.pages_.setCursorByObject(newPage);
            createSelectedPage();
        }
    }
    break;

    case PAGE_UPDATE:
    {
        PageShell *pageForAction = getPageByKey(data->getParameter(0));
        if (pageForAction == pages.openedPage_ && !pageForAction->isOpened())
        {
            pages.openedPage_->createPage();
            display.setBrightness(settings.getValueByKey(SETUP_BRIGHTNESS));
        }
        if (pageForAction->isCreated())
        {
            pageForAction->getPage()->onAction(data);
        }
    }
    break;

    case PAGE_SLEEP:
        pages.openedPage_->Dispose();
        display.setBrightness(0);
        break;

    case PAGE_WAKE_UP:
        if (!pages.openedPage_->isOpened() && pages.notify_ == nullptr)
            createSelectedPage();
        break;

    case PAGE_CREATE_NOTIFY:
        ENTER_NOTIFY;
        if (pages.notifyParams_ != nullptr && pages.notify_ == nullptr)
        {
            pages.openedPage_->Dispose();
            pages.notify_ = new Notification(pages.notifyParams_);
            pages.notify_->showNotifyName();
            energy.updateLastActiveTime();
            display.setBrightness(settings.getValueByKey(SETUP_BRIGHTNESS));
        }
        EXIT_NOTIFY;
        break;
    default:
        break;
    }
    EXIT_PAGES;
}

PageShell *PagesController::getOpenedPage()
{
    ENTER_PAGES;
    PageShell *page = openedPage_;
    EXIT_PAGES;
    return page;
}

void PagesController::createSelectedPage()
{
    display.setBrightness(0);
    pages.openedPage_->Dispose();
    pages.openedPage_ = pages.pages_.getCursorItem();
    ble.send(CMDKEY_OPEN_PAGE, {pages.openedPage_->getKey()});
    pages.openedPage_->createPage();
    display.setBrightness(settings.getValueByKey(SETUP_BRIGHTNESS));
}

PageShell *PagesController::getPageByKey(char *key, uint16_t *index)
{
    PageShell *newPage = pages.pages_.getBy([key](PageShell *data) -> bool
                                            { return !(bool)strcmp(data->getKey(), key); },
                                            index);
    return newPage;
}

void PagesController::setNotificationEnabled(bool isEnabled)
{
    ENTER_NOTIFY;
    isNotificationEnabled_ = isEnabled;
    EXIT_NOTIFY;
}

PagesController &pages = PagesController::getInstance();
