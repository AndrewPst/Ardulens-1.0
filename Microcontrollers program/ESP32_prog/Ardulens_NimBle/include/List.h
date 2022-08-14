// #ifndef __LIST1__
// #define __LIST1__

// #include <Arduino.h>

// template <typename T>
// class List
// {
// public:
//     List()
//     {
//         size_ = 0;
//         head = nullptr;
//     }
//     ~List()
//     {
//         //Serial << "Вызвался дестркутор объекта класса List" << endl;
//         clear();
//     }

//     T *push_back(T *data)
//     { // вставка в конец списка

//         if (head == nullptr)
//         {
//             head = new Node<T>(data);
//         }
//         else
//         {
//             Node<T> *current = this->head;
//             while (current->pNext != nullptr)
//             {
//                 current = current->pNext;
//             }
//             current->pNext = new Node<T>(data);
//         }
//         size_++;
//         return data;
//     }
//     void push_front(T *data)
//     { // добавить в начало списка
//         head = new Node<T>(data, head);
//         size_++;
//     }
//     void insert(T *data, int index)
//     { // добавить в списка согласно индекса
//         if (index == 0)
//         {
//             push_front(data);
//         }
//         else
//         {
//             Node<T> *prev = this->head;
//             for (int i = 0; i < index - 1; i++)
//             {
//                 prev = prev->pNext;
//             }
//             Node<T> *newNode = new Node<T>(data, prev->pNext);
//             prev->pNext = newNode;
//         }
//         size_++;
//     }
//     T *removeAt(int index, bool toOut = false)
//     { // // удалить по индексу. toOut - если True, то не удаляется из ОЗУ, а возвращается из запроса
//         if (index == 0)
//         {
//             return pop_front(toOut);
//         }
//         else
//         {
//             Node<T> *prev = this->head;
//             for (int i = 0; i < index - 1; i++)
//             {
//                 prev = prev->pNext;
//             }
//             Node<T> *toDelete = prev->pNext;
//             prev->pNext = toDelete->pNext;

//             size_--;

//             if (toOut)
//             {
//                 T *answer = toDelete->data; // указатель на данные
//                 delete toDelete;
//                 return answer; // вернуть указатель на данные
//             }
//             else
//             {
//                 // удалить из ОЗУ объект data по его указателю в узле toDelete
//                 delete toDelete->data;
//                 delete toDelete; // удалить из ОЗУ просто узел
//                 return nullptr;
//             }
//         }
//     }
//     T *pop_front(bool toOut = false)
//     { // удаление первого элемента

//         Node<T> *temp = head; // head тут у нас не объект (!), а УКАЗАТЕЛЬ на УЗЕЛ, у которого поле data - это тоже указатель на другой объект другого класса
//         T *data = head->data; // указатель на объект data (ведь дата у нас не просто данные, а УКАЗАТЕЛЬ на объект, и этот объект тоже надо удолять из ОЗУ)

//         head = head->pNext;

//         size_--;
//         if (toOut)
//         {

//             return data;
//         }
//         delete data;
//         delete temp; // очистка ОЗУ от объекта data класса <T>
//         return nullptr;
//     }
//     T *pop_back(bool toOut = false)
//     { // удалить последний элемент
//         return removeAt(size_ - 1, toOut);
//     }

//     void clear()
//     { // очистка всего списка
//         int s = this->size_;
//         while (s-- > 0)
//         {
//             pop_front(false); // удалить без возвращения удаляемого элемента
//         }
//     }

//     int length()
//     {
//         return size_;
//     }

//     T *get(int index) // получить указатель на элемент по его индексу
//     {
//         // проверка на допустимость величины индекса
//         if (index > this->size_ - 1)
//             return nullptr;
//         int counter = 0;
//         Node<T> *current = this->head; // создаем временный указатель здесь сущесвующий head это объект класс Node

//         int s = this->size_;
//         while (counter < s)
//         { // крутиться пока счетчик меньше размера списка

//             if (counter == index)
//             {
//                 return (current->data);
//             }
//             else
//             {
//                 current = current->pNext; // pNext здесь УКАЗАТЕЛЬ
//                 counter++;
//             }
//         }
//         return nullptr;
//     }

//     T *getBy(std::function<bool(T* data)> pFunc, uint16_t *object_index = nullptr)
//     { // возвращает элемент списка согласно функции выборки pFunc
//         int counter = 0;
//         int s = this->size_;
//         Node<T> *current = this->head;
//         while (counter < s)
//         {
//             if (pFunc(current->data))
//             {
//                 if (object_index != nullptr)
//                     *object_index = counter;
//                 return current->data;
//             }
//             else
//             {
//                 current = current->pNext; // pNext здесь УКАЗАТЕЛЬ
//                 counter++;
//             }
//             delay(1);
//         }
//         if (object_index != nullptr)
//             *object_index = 0;
//         return nullptr;
//     }

//     T *operator[](int index)
//     {
//         return get(index);
//     }

//     void setCursorPos(uint16_t pos)
//     {
//         cursor = pos >= size_ ? size_ - 1 : pos;
//     }
//     T *cursorChange(int8_t direct)
//     {
//         if (cursor == 0 && direct < 0)
//         {
//             cursor = 0;
//         }
//         else
//         {
//             cursor = cursor + direct;
//         }

//         if (cursor > size_ - 1)
//             cursor = size_ - 1;
//         if (cursor < 0)
//             cursor = 0;
//         return getCursorItem();
//     }
//     T *getNextItem()
//     {
//         return cursorChange(1);
//     }
//     T *getPrevItem()
//     {
//         return cursorChange(-1);
//     }
//     T *getCursorItem()
//     {
//         return this->get(this->cursor);
//     }

// private:
//     uint16_t size_ = 0;
//     uint8_t *level;
//     int8_t cursor = 0; // значение курсора
//     // внутренний класс. Узел
//     template <typename U>
//     class Node
//     {
//     public:
//         Node *pNext;
//         T *data; // УКАЗАТЕЛЬ на объект

//         Node(T *data = T(), Node *pNext = nullptr)
//         {
//             this->data = data;
//             this->pNext = pNext;
//         }
//         ~Node() {}
//     };

//     Node<T> *head; // головной элемент
// };

// #endif