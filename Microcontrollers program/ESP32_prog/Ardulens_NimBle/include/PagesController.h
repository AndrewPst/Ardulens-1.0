#ifndef _DISPLAY_CONTROLLER
#define _DISPLAY_CONTROLLER

#include <Arduino.h>
#include <Setup.h>

#include <Display.h>
#include <DataParser.h>
#include <BleController.h>
#include <Page.h>
#include <Settings.h>
#include <DoubleRingList.h>
#include <ButtonsController.h>
#include <EnergyController.h>
#include <Settings.h>
#include <Notification.h>


#define APP_INDEX_IN_PARAM 0

enum PageAction : uint8_t
{
    PAGE_UPDATE = 1,
    PAGE_NEW,
    PAGE_NEXT,
    PAGE_PREVIOUS,
    PAGE_HOME,
    PAGE_SLEEP,
    PAGE_WAKE_UP,
    PAGE_CREATE_NOTIFY
};

class PagesController final
{
private:
    // делаем синглтон
    PagesController();
    PagesController(const PagesController &) = delete;
    PagesController &operator=(PagesController &) = delete;

    SemaphoreHandle_t pagesMutex_;  //мютех обращений к страницам
    SemaphoreHandle_t notifyMutex_; //мютех обращений к уведомлениям

    DoubleCycleList<PageShell> pages_; //список страниц

    TaskHandle_t pageCycleTaskHundle_; //handle графического потока

    QueueHandle_t queue_; //очередь задач графического потока

    bool isNotificationEnabled_ = true;
    NotificationParameters_t *notifyParams_; //параметры текущего уведомления
    Notification *notify_;                   //текущее уведомление

    uint16_t homePageIndex_; //индекс открытой страницы
    PageShell *openedPage_;  //указатель на открытую страницу

    PageAction action_; //Переменная для записи поступившего действия

    /*
    * Цикл, в котором обрабатывается основная логика графического интерфейса и работы страниц.
    *
    * @param param аргумент для передачи в функцию (не используется)
    */
    static void pageCycle(void *param);

    /*
    * Метод выполняет обработку поступивших данных.
    *
    * @param action действие, которое нужно обработать.
    * @param data данные, полученные по ble.
    */
    static void PerformAction(PageAction action, DataParser *data);

    /*
    * Метод находит нужную страницу по ее ключу
    *
    * @param key ключ страницы.
    * @param index индекс найденной страницы.
    * 
    * @return 'nullptr' если страница не найдена, указатель на страницу, если найдена.
    */
    static PageShell *getPageByKey(char *key, uint16_t *index = nullptr);

    /*
    * событие открытия новой страницы.
    *
    * @param data полученные данные.
    */
    static void OnOpenPageCommandReceived(DataParser *data);

    /*
    * событие получения данных для открытой страницы.
    *
    * @param data полученные данные.
    */
    static void OnUpdatePageCommandReceived(DataParser *data);

    /*
    * событие изменения режима энергосбережения.
    *
    * @param mode режим энергосбережения.
    */
    static void OnEnergyStateChanged(bool mode);

    /*
    * событие регистрации свайпа.
    *
    * @param swipe тип свайпа.
    */
    static void OnSwipeRegistered(SwipeType_t swipe);

    /*
    * событие создания уведомления.
    *
    * @param data полученные данные.
    */
    static void OnCommandCreateNotifyRegistred(DataParser *data);

    /*
    * Создать выбранную курсором страницу 
    */
    static void createSelectedPage();

    /*
    * Обработать событие Tick у страницы. 
    */
    static void refreshPageTime();

    /*
    * Удалить открытое уведомление
    */
    static void deleteOpenedNotify();

    /*
    * обновление уведомления
    */
    static void updateNotify();

    /*
    * инициализация страниц
    */
    void initPages();

public:
    /*
    * Возращает экземпляр класса
    * @return Экземпляр класса
    */
    static PagesController &getInstance();

    /*
    * Инициализация класса
    */
    void Initialize();

    /*
    * Открыть следующую страницу
    */
    void OpenNextPage();

    /*
    * Открыть предыдущую страницу
    */
    void OpenPreviousPage();

    /*
    * Открыть домашнюю страницу
    */
    void OpenHomePage();

    /*
    * создать уведомление
    * @param notify параметры уведомления
    */
    void createNotification(NotificationParameters_t *notify);

    /*
    * Получить указатель на открытую страницу
    */
    PageShell *getOpenedPage();

    void setNotificationEnabled(bool isEnabled);
    
};

extern PagesController &pages;

#endif //_DISPLAY_CONTROLLER