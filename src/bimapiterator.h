#pragma once

#include "bimappair.h"

#include <set>

template<typename Key, typename T> class BiMap;

//! Итератор двунаправленного ассоциативного контейнера, построенного на базе std::set
template<typename Key, typename T>
class BiMapIterator : public std::iterator<std::bidirectional_iterator_tag, std::pair<Key, T>>
{
    template<typename BiMapKey, typename BiMapT> friend class BiMap;

public:
    //! Тип значения
    using value_type = BiMapPair<Key, T>;

    //! Оператор неравенства
    bool operator !=(const BiMapIterator &other) const;

    //! Оператор равенства
    bool operator ==(const BiMapIterator &other) const;

    //! Оператор инкремента
    BiMapIterator<Key, T> &operator++();

    //! Оператор декремента
    BiMapIterator<Key, T> &operator--();

    //! Оператор инкремента
    BiMapIterator<Key, T> operator++(int);

    //! Оператор декремента
    BiMapIterator<Key, T> operator--(int);

    //! Оператор разименования
    std::pair<Key, T> &operator *() const;

    //! Оператор доступа к членам класса
    std::pair<Key, T> *operator->() const;

private:
    //! Итератор значения
    using it_value = BiMapPair<std::remove_const_t<Key>, std::remove_const_t<std::remove_reference_t<T>>&>;

    //! Конструктор
    BiMapIterator(const typename std::set<it_value>::iterator &it) : _it(it) {}

    //! Итератор
    typename std::set<it_value>::iterator _it;
};


template<typename Key, typename T>
bool BiMapIterator<Key, T>::operator!=(const BiMapIterator &other) const
{
    return _it != other._it;
}

template<typename Key, typename T>
bool BiMapIterator<Key, T>::operator==(const BiMapIterator &other) const
{
    return _it == other._it;
}

template<typename Key, typename T>
BiMapIterator<Key, T> &BiMapIterator<Key, T>::operator++()
{
    ++_it;
    return *this;
}

template<typename Key, typename T>
BiMapIterator<Key, T> &BiMapIterator<Key, T>::operator --()
{
    --_it;
    return *this;
}

template<typename Key, typename T>
BiMapIterator<Key, T> BiMapIterator<Key, T>::operator++(int)
{
    auto old = *this;
    operator++();
    return old;
}

template<typename Key, typename T>
BiMapIterator<Key, T> BiMapIterator<Key, T>::operator --(int)
{
    auto old = *this;
    operator--();
    return old;
}

template<typename Key, typename T>
std::pair<Key, T> &BiMapIterator<Key, T>::operator *() const
{
    auto value = const_cast<it_value *>(&*_it);
    return reinterpret_cast<std::pair<Key, T> &>(*value);
}

template<typename Key, typename T>
std::pair<Key, T> *BiMapIterator<Key, T>::operator ->() const
{
    auto *value = const_cast<it_value *>(_it.operator->());
    return reinterpret_cast<std::pair<Key, T> *>(value);
}
