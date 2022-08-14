#ifndef __DISPLAY__
#define __DISPLAY__

#include <Arduino.h>

#include <TFT_eSPI.h>
#include <JPEGDecoder.h>
#include <Setup.h>

class Display : public TFT_eSPI
{
private:
    Display();
    Display(const Display &) = delete;
    Display &operator=(const Display &) = delete;


    void jpegRender(int16_t xpos, int16_t ypos);
    void jpegRenderToSprite(TFT_eSprite *sprite, int16_t xpos, int16_t ypos);

public:
    static Display &getInstance();

    void setupLedPin();
    void setBrightness(uint8_t value);

    void resetAllViewport();
    void drawJpeg(const char *filename, int16_t xpos, int16_t ypos, TFT_eSprite *sprite = nullptr);

    void setFontSize(uint16_t size);

    //char *utf8rus2(char *source);

    void convertUTF8(char *outputBuffer, char *inputBuffer);
    char* convertUTF8(char* input);
};

extern Display &display;

#endif