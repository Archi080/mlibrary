
#include "MoviesException.h"

#ifndef MOVIELIBRARY_LIST_H
#define MOVIELIBRARY_LIST_H

template<class T>
class CircularList{ //Кольцевой список
public:
    //Добавление элемента
    void add(T * element){
        //Создаем новый элемент
        auto newElement = new Element(element);
        //Если элементы отсутствуют...
        if (this->first == nullptr){
            //Первым элементом будет новый
            this->first = newElement;
            //Закольцовываем список
            newElement->next = this->first;
        }
        else
        {
            //Сохраняем первый элемент
            auto temp = this->first;
            //Ищем последний элемент
            while (temp->next != this->first){
                temp = temp->next;
            }
            //Добавляем новый элемент в начало списка
            newElement->next = this->first;
            //Связываем последний элемент с первым
            temp->next = newElement;
            //Новый элемент становится первым
            this->first = newElement;
        }
    }

    //Переопределение оператора []
    T * operator[](int index){
        //Если отсутствуют элементы или индекс отрицательный
        //выбрасываем исключение
        if (first == nullptr || index < 0) throw bad_addr_exception();
        //Выбираем первый элемент
        auto current = this->first;
        //Переходим к последующему,
        //пока не дойдем до нужного элемента
        for(int i = 0; i < index; i++){
            current = current->next;
        }
        //Возвращаем данные из соответствующего элемента
        return current->data;
    }

    //Извлечение элемента
    T * pop(){
        //Если отсутствуют - элементы выбрасываем исключение
        if (first == nullptr) throw bad_addr_exception();
        //Сохраняем первый элемент
        auto ptr = this->first;
        //Если в списке только один элемент
        if (first->next == first){
            //Удаляем первый элемент
            this->first = nullptr;
            //Возвращаем данные
            return ptr->data;
        }
        //Если элементов более одного
        else{
            //Двигаемся к концу списка
            while (ptr->next != this->first) {
                ptr = ptr->next;
            }
            //Замыкаем последний элемент со вторым
            ptr->next = first->next;
            //Сохраняем первый элемент
            auto toReturn = this->first;
            //Устанавливаем на место начала списка второй элемент
            this->first = ptr->next;
            //Возвращаем данные
            return toReturn->data;
        }

    }

    bool isEmpty(){
        return !first;
    }

private:
    //Структура элемента кольцевого списка
    struct Element{
        //Конструктор структуры элемента
        explicit Element(T * data){
            this->data = data;
        }
        //Данные
        T * data = nullptr;
        //Адрес следующего элемента
        Element * next = nullptr;
    };
    //Адрес первого элемента
    Element * first = nullptr;
};

//Односвязный список
template<class T>
class List{
public:
    //Получение размера списка
    int getSize(){
        return this->size;
    }

    //Очистка списка
    void clear(){
        int toDelete = this->size;
        for (int i = 0; i < toDelete; i++){
            remove(0);
        }
    }

    //Добавление элемента по индексу
    void insert(int index, T * element) {
        //Если индекс больше размера массива
        //или отрицательный выбрасываем исключение
        if (index >= this->size || index < 0) throw bad_addr_exception();
        //Создаем новый элемент
        auto newElement = new Element(element);
        //Вставка в начало списка
        if (index == 0){
            newElement->next = this->first;
            this->first = newElement;
            this->size++;
        }
        //Вставка в конец списка
        else if (index == size - 1){
            this->add(element);
        }
        //Вставка в середину
        else
        {
            //Ищем соответствующий элемент
            auto current = this->first;
            for(int i = 0; i < index - 1; i++){
                current = current->next;
            }
            //Вставляем новый элемент после найденного
            newElement->next = current->next;
            current->next = newElement;
            this->size++;
        }
    }

    //Добавление элемента в конец списка
    void add(T * element) {
        //Создаем новый элемент
        auto newElement = new Element(element);
        //Если список не пустой
        if(this->size != 0){
            this->last->next = newElement;
            this->last = newElement;
        }
        //Если пустой
        else
        {
            this->first = newElement;
            this->last = newElement;
        }
        this->size++;
    }

    //Удаление элемента из списка
    void remove(int index){
        //Если не существует такого элемента выбрасываем исключение
        if (index >= this->size || index < 0) throw bad_addr_exception();
        //Удаление элемента из начала
        if(index == 0){
            auto toDelete = this->first;
            this->first = this->first->next;
            delete toDelete;
            this->size--;
        }
        //Удаление элемента с конца
        else if(index == size - 1){
            auto toDelete = this->last;
            auto current = this->first;
            while(current->next != toDelete){
                current = current->next;
            }
            current->next = nullptr;
            this->last = current;
            delete toDelete;
            this->size--;
        }
        //Удаление элемента из середины
        else{
            auto current = this->first;
            for(int i = 0; i < index - 1; i++){
                current = current->next;
            }
            auto toDelete = current->next;
            current->next = current->next->next;
            delete toDelete;
            this->size--;
        }
    }

    //Переопределение оператора []
    T * operator[](int index) {
        if (index >= this->size) throw bad_addr_exception();
        auto current = this->first;
        for(int i = 0; i < index; i++){
            current = current->next;
        }
        return current->data;
    }

    //Переопределение оператора +=
    void operator+=(List<T> list){
        for (int i = 0; i < list.getSize(); i++){
            this->add(list[i]);
        }
        this->size += list.size;
    }

private:
    //Структура элемента
    struct Element{
        //Конструктор элемента
        explicit Element(T * data){
            this->data = data;
        }
        T * data = nullptr;
        Element * next = nullptr;
    };
    //Адрес первого и последнего элементов
    Element * first = nullptr;
    Element * last = nullptr;
    //Количество элементов
    int size = 0;
};

#endif //MOVIELIBRARY_LIST_H
