#ifndef ReceivedData_h
#define ReceivedData_h

#include <Arduino.h>

class DataParser
{
public:
    // передать char array строку, можно указать символ разделитель
    DataParser(char newDiv = '|');
    ~DataParser();

    // указать символ разделитель
    void setDiv(char newDiv);

    // разделить строку на подстроки
    uint16_t parse(char *buf);

    uint16_t getLenght();

    // получить инт из выбранной подстроки
    int16_t getInt(int num);

    // получить float из выбранной подстроки
    float getFloat(int num);

    // сравнить подстроку с другой строкой
    bool equals(int num, const char *comp);

    char *getParameter(uint16_t idx);

    char *getKey();

    char* getFlagStr();

    char *operator[](uint16_t idx);

private:
    // количество разделённых данных в пакете
    uint16_t amount(char* buf);

    // освободить буфер
    void clear();

    uint16_t lenght_ = 0;

    char div_;
    char **str_ = NULL;

    SemaphoreHandle_t mutex_;
};

#endif