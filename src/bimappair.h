#pragma once

#include <utility>

//! Класс пары значений со сравнением только первого значения
template< typename T1, typename T2>
struct BiMapPair : public std::pair<T1, T2>
{
    //! Конструктор по умолчанию
    BiMapPair() : std::pair<T1, T2>() {}

    //! Конструктор
    BiMapPair(const T1& x, const T2& y) : std::pair<T1, T2>(x, y) {}

    //! Конструктор
    BiMapPair(const T1&& x, const T2&& y) : std::pair<T1, T2>(x, y) {}

    //! Оператор равенства
    friend bool operator==(const BiMapPair<T1, T2> &lhs, const BiMapPair<T1, T2> &rhs)
    {
        return lhs.first == rhs.first;
    }

    //! Оператор меньше
    friend bool operator<(const BiMapPair<T1,T2> &lhs, const BiMapPair<T1,T2> &rhs)
    {
        return lhs.first < rhs.first;
    }
};
