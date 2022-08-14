#ifndef _PAGES_
#define _PAGES_

#include <Arduino.h>
#include <ButtonsController.h>
#include <DataParser.h>

class PageBase
{


public:

    virtual void onCreate();
    virtual void onAction(DataParser *data);
    virtual void onButtonClicked(ButtonAction_t button, ButtonKeys key);
    virtual void onMotionDetected(uint8_t motionType);
    virtual void onTick();
    virtual void onDestroy();
    virtual ~PageBase() = default;
};

class PageShell
{

protected:
    PageBase *page_;

private:
    static QueueHandle_t buttonsQueue;
    static void OnButtonClicked(ButtonStateChangedEventArgs state);
    uint32_t pageOpeningTime = 0;
public:
    static void init();

    PageShell() = default;

    PageBase *getPage();
    void createPage();

    virtual ~PageShell() = default;

    virtual PageBase *GetInstancePage() = 0;
    virtual const char *getKey() = 0;

    void tick();
    virtual bool isCreated();
    virtual bool isOpened();
    virtual void Dispose();
};

#endif