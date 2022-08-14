#ifndef _EVENTS_H_
#define _EVENTS_H_

#include <Arduino.h>

#include <List.h>
#include <map>
#include <iterator>

//Самодельная реализация событий из c#. 
template <typename A>
class Events
{
public:
    typedef void (*SubFunction)(A args);

    Events()
    {
    }
    void subscribe(SubFunction func)
    {
        subscribers_.push_back(func);
    }
    void unsubscribe(SubFunction func) //не тестировался
    {
        uint16_t index = 0;
        SubFunction value = subscribers_.getBy([func](SubFunction *data)
                                               {
                                                   if (func == data)
                                                       return true;
                                                   return false;
                                               },
                                               &index);
        if (value != nullptr)
        {
            subscribers_.removeAt(index);
        }
    }

    void Invoke(A eventArgs)
    {
        SubFunction sub_func;
        for (uint16_t i = 0; i < subscribers_.size(); i++)
        {
            sub_func = subscribers_[i];
            sub_func(eventArgs);
            yield();
        }
    }

    void operator+=(const SubFunction func)
    {
        subscribe(func);
    }

    void operator-=(const SubFunction func)
    {
        unsubscribe(func);
    }

private:
    std::vector<SubFunction> subscribers_;
};


//Версия событий с проверкой ключа. Чтобы 100500 методов не проверяли одно и тоже
template <typename K, typename A>
class EventsMap
{
public:
    typedef void (*SubFunctionMap)(A args);

    EventsMap()
    {
    }
    void subscribe(K key, SubFunctionMap func)
    {
        subscribers_[key] = func;
    }

    void unsubscribe(K key) //не тестировался
    {
        subscribers_.erase(key);
    }

    void Invoke(K key, A args)
    {
        auto func = subscribers_.find(key);

        if (func != subscribers_.end())
        {
            func->second(args);
        }
    }

private:
    std::map<K, SubFunctionMap> subscribers_;
};
#endif