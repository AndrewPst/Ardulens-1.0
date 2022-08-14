#include <Display.h>

Display &display = Display::getInstance();

#define KOEF_SPEED 30
#define PAUSE_TIME 5
#define LED_CHANNEL 0

#include <Fonts/FontsRus/FreeSans6.h>
#include <Fonts/FontsRus/FreeSans7.h>
#include <Fonts/FontsRus/FreeSans8.h>
#include <Fonts/FontsRus/FreeSans9.h>
#include <Fonts/FontsRus/FreeSans10.h>
#include <Fonts/FontsRus/FreeSans12.h>
#include <Fonts/FontsRus/FreeSans14.h>
#include <Fonts/FontsRus/FreeSans16.h>
#include <Fonts/FontsRus/FreeSans18.h>

const GFXfont *fontArray[9]{
    &FreeSans6pt8b,
    &FreeSans7pt8b,
    &FreeSans8pt8b,
    &FreeSans9pt8b,
    &FreeSans10pt8b,
    &FreeSans12pt8b,
    &FreeSans14pt8b,
    &FreeSans16pt8b,
    &FreeSans18pt8b,
};

Display::Display() : TFT_eSPI(80, 160)
{
}

Display &Display::getInstance()
{
    static Display dp;
    return dp;
}

void Display::setupLedPin()
{
    ledcSetup(LED_CHANNEL, 50000, 8);
    ledcAttachPin(DISPLAY_LED_PIN, LED_CHANNEL);
    ledcWrite(LED_CHANNEL, 0);
}

void Display::setBrightness(uint8_t value)
{

    uint8_t cur = ledcRead(LED_CHANNEL);
    int16_t dif = (int16_t)((value - cur) / KOEF_SPEED);
    if (dif == 0)
    {
        dif = value > cur ? +1 : -1;
    }
    bool flag = false;
    int16_t led = cur;
    while (!flag)
    {
        int16_t nled = led + dif;
        if ((dif > 0) ? (nled > value) : (nled < value))
        {
            led = value;
            flag = true;
        }
        else
        {
            led = nled;
        }
        ledcWrite(LED_CHANNEL, led);
        delay(PAUSE_TIME);
    }
}

void Display::setFontSize(uint16_t size)
{
    setFreeFont(fontArray[size]);
}

void Display::resetAllViewport()
{
    resetViewport();
    display.setViewport(DISPLAY_OFFSET, 0, 160 - DISPLAY_OFFSET, 80);
}

void Display::drawJpeg(const char *filename, int16_t xpos, int16_t ypos, TFT_eSprite *sprite)
{
    File jpegFile = SPIFFS.open(filename, FILE_READ); // or, file handle reference for SD library
    // Use one of the following methods to initialise the decoder:
    bool decoded = JpegDec.decodeSdFile(jpegFile); // Pass the SD file handle to the decoder,
    //bool decoded = JpegDec.decodeSdFile(filename);  // or pass the filename (String or character array)
    if (decoded)
    {
        // render the image onto the screen at given coordinates
        if (sprite == nullptr)
            jpegRender(xpos, ypos);
        else
            jpegRenderToSprite(sprite, xpos, ypos);
    }
    jpegFile.close();
}

void Display::jpegRender(int16_t xpos, int16_t ypos)
{
    uint16_t *pImg;
    uint16_t mcu_w = JpegDec.MCUWidth;
    uint16_t mcu_h = JpegDec.MCUHeight;
    uint32_t max_x = JpegDec.width;
    uint32_t max_y = JpegDec.height;

    bool swapBytes = getSwapBytes();
    setSwapBytes(true);

    // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
    // Typically these MCUs are 16x16 pixel blocks
    // Determine the width and height of the right and bottom edge image blocks
    uint32_t min_w = jpg_min(mcu_w, max_x % mcu_w);
    uint32_t min_h = jpg_min(mcu_h, max_y % mcu_h);

    // save the current image block size
    uint32_t win_w = mcu_w;
    uint32_t win_h = mcu_h;
    // save the coordinate of the right and bottom edges to assist image cropping
    // to the screen size
    max_x += xpos;
    max_y += ypos;

    // Fetch data from the file, decode and display
    while (JpegDec.read())
    {                          // While there is more data in the file
        pImg = JpegDec.pImage; // Decode a MCU (Minimum Coding Unit, typically a 8x8 or 16x16 pixel block)

        // Calculate coordinates of top left corner of current MCU
        int16_t mcu_x = JpegDec.MCUx * mcu_w + xpos;
        int16_t mcu_y = JpegDec.MCUy * mcu_h + ypos;

        // check if the image block size needs to be changed for the right edge
        if (mcu_x + mcu_w <= max_x)
            win_w = mcu_w;
        else
            win_w = min_w;

        // check if the image block size needs to be changed for the bottom edge
        if (mcu_y + mcu_h <= max_y)
            win_h = mcu_h;
        else
            win_h = min_h;

        // copy pixels into a contiguous block
        if (win_w != mcu_w)
        {
            uint16_t *cImg;
            uint16_t p = 0;
            cImg = pImg + win_w;
            for (int h = 1; h < win_h; h++)
            {
                p += mcu_w;
                for (int16_t w = 0; w < win_w; w++)
                {
                    *cImg = *(pImg + w + p);
                    cImg++;
                }
            }
        }

        // draw image MCU block only if it will fit on the screen
        if ((mcu_x + win_w) <= width() && (mcu_y + win_h) <= height())
            pushImage(mcu_x, mcu_y, win_w, win_h, pImg);
        else if ((mcu_y + win_h) >= height())
            JpegDec.abort(); // Image has run off bottom of screen so abort decoding
    }
    setSwapBytes(swapBytes);
}

void Display::jpegRenderToSprite(TFT_eSprite *sprite, int16_t xpos, int16_t ypos)
{
    uint16_t *pImg;
    uint16_t mcu_w = JpegDec.MCUWidth;
    uint16_t mcu_h = JpegDec.MCUHeight;
    uint32_t max_x = JpegDec.width;
    uint32_t max_y = JpegDec.height;

    bool swapBytes = sprite->getSwapBytes();
    sprite->setSwapBytes(true);

    // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
    // Typically these MCUs are 16x16 pixel blocks
    // Determine the width and height of the right and bottom edge image blocks
    uint32_t min_w = jpg_min(mcu_w, max_x % mcu_w);
    uint32_t min_h = jpg_min(mcu_h, max_y % mcu_h);

    // save the current image block size
    uint32_t win_w = mcu_w;
    uint32_t win_h = mcu_h;

    // save the coordinate of the right and bottom edges to assist image cropping
    // to the screen size
    max_x += xpos;
    max_y += ypos;

    // Fetch data from the file, decode and display
    while (JpegDec.read())
    {                          // While there is more data in the file
        pImg = JpegDec.pImage; // Decode a MCU (Minimum Coding Unit, typically a 8x8 or 16x16 pixel block)

        // Calculate coordinates of top left corner of current MCU
        int16_t mcu_x = JpegDec.MCUx * mcu_w + xpos;
        int16_t mcu_y = JpegDec.MCUy * mcu_h + ypos;

        // check if the image block size needs to be changed for the right edge
        if (mcu_x + mcu_w <= max_x)
            win_w = mcu_w;
        else
            win_w = min_w;

        // check if the image block size needs to be changed for the bottom edge
        if (mcu_y + mcu_h <= max_y)
            win_h = mcu_h;
        else
            win_h = min_h;

        // copy pixels into a contiguous block
        if (win_w != mcu_w)
        {
            uint16_t *cImg;
            uint16_t p = 0;
            cImg = pImg + win_w;
            for (int h = 1; h < win_h; h++)
            {
                p += mcu_w;
                for (int16_t w = 0; w < win_w; w++)
                {
                    *cImg = *(pImg + w + p);
                    cImg++;
                }
            }
        }

        // draw image MCU block only if it will fit on the screen
        if ((mcu_x + win_w) <= sprite->width() && (mcu_y + win_h) <= sprite->height())
            sprite->pushImage(mcu_x, mcu_y, win_w, win_h, pImg);
        else if ((mcu_y + win_h) >= sprite->height())
            JpegDec.abort(); // Image has run off bottom of screen so abort decoding
    }
    sprite->setSwapBytes(swapBytes);
}

// #define maxString 100 // ограничиваем строку шириной экрана
// char *Display::utf8rus2(char *source)
// {
//     // if (target != NULL)
//     //     delete[] target;
//     target = new char[maxString + 1];
//     int i, j, k;
//     unsigned char n;
//     char m[2] = {'0', '\0'};
//     strcpy(target, "");
//     k = strlen(source);
//     i = j = 0;
//     while (i < k)
//     {
//         n = source[i];
//         i++;
//         if (n >= 127)
//         {
//             switch (n)
//             {
//             case 208:
//             {
//                 n = source[i];
//                 i++;
//                 if (n == 129)
//                 {
//                     n = 192;
//                     break;
//                 } // перекодируем букву Ё
//                 break;
//             }
//             case 209:
//             {
//                 n = source[i];
//                 i++;
//                 if (n == 145)
//                 {
//                     n = 193;
//                     break;
//                 } // перекодируем букву ё
//                 break;
//             }
//             }
//         }
//         m[0] = n;
//         strcat(target, m);
//         j++;
//         if (j >= maxString)
//             break;
//     }
//     return target;
// }

void Display::convertUTF8(char *outputBuffer, char *inputBuffer)
{
    int i, j, k;
    unsigned char n;
    char m[2] = {'0', '\0'};
    strcpy(outputBuffer, "");
    k = strlen(inputBuffer);
    i = j = 0;
    while (i < k)
    {
        n = inputBuffer[i];
        i++;

        if (n >= 127)
        {
            switch (n)
            {
            case 208:
            {
                n = inputBuffer[i];
                i++;
                if (n == 129)
                {
                    n = 192;
                    break;
                } // перекодируем букву Ё
                break;
            }
            case 209:
            {
                n = inputBuffer[i];
                i++;
                if (n == 145)
                {
                    n = 193;
                    break;
                } // перекодируем букву ё
                break;
            }
            }
        }
        m[0] = n;
        strcat(outputBuffer, m);
        j++;
    }
}

char *Display::convertUTF8(char *input)
{
    char *result = new char[strlen(input) + 1];
    convertUTF8(result, input);
    return result;
}
