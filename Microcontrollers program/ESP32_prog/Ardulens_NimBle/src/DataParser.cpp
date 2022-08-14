#include <DataParser.h>

#define ENTER xSemaphoreTake(mutex_, portMAX_DELAY)
#define EXIT xSemaphoreGive(mutex_)

DataParser::DataParser(char newDiv)
{
    mutex_ = xSemaphoreCreateMutex();
    div_ = newDiv;
}

DataParser::~DataParser()
{
    clear();
}

void DataParser::setDiv(char newDiv)
{
    div_ = newDiv;
}

void DataParser::clear()
{
    ENTER;
    if (str_)
        free(str_);
    EXIT;
}

uint16_t DataParser::amount(char *buf)
{
    uint16_t i = 0, count = 0;
    ENTER;
    while (buf[i++])
        if (buf[i] == div_)
            count++; // подсчёт разделителей
    lenght_ = ++count;
    EXIT;
    return lenght_;
}

uint16_t DataParser::getLenght()
{
    ENTER;
    auto value = lenght_;
    EXIT;
    return value;
}

uint16_t DataParser::parse(char *buf)
{
    clear();                   // освобождаем буфер
    uint16_t am = amount(buf); // количество данных
    ENTER;
    str_ = (char **)malloc(am * sizeof(char *)); // создаём буфер
    str_[0] = buf;                               // строка 0
    int i = 0, j = 0;                            // счётчики
    while (buf[i])
    { // пока не NULL
        if (buf[i] == div_)
        {                            // если разделитель
            buf[i] = '\0';           // меняем на NULL
            str_[++j] = buf + i + 1; // запоминаем начало строки
        }
        i++;
    }
    EXIT;
    return am;
}

int16_t DataParser::getInt(int num)
{
    ENTER;
    auto value = atol(str_[num]);
    EXIT;
    return value;
}

float DataParser::getFloat(int num)
{
    ENTER;
    auto value = atof(str_[num]);
    EXIT;
    return value;
}

// сравнить подстроку с другой строкой
bool DataParser::equals(int num, const char *comp)
{
    ENTER;
    auto value = !strcmp(str_[num], comp);
    EXIT;
    return value;
}

char *DataParser::getParameter(uint16_t idx)
{
    if (idx + 3 >= lenght_)
        return nullptr;
    ENTER;
    auto value = str_[idx + 2];
    EXIT;
    return value;
}

char *DataParser::getKey()
{
    if (lenght_ < 2)
        return nullptr;
    ENTER;
    auto value = str_[1];
    EXIT;
    return value;
}

char *DataParser::getFlagStr()
{
    ENTER;
    auto value = str_[0];
    EXIT;
    return value;
}

char *DataParser::operator[](uint16_t idx)
{
    return getParameter(idx);
}
