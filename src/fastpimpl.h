#ifndef FASTPIMPL_H
#define FASTPIMPL_H

#include <cstddef>
#include <utility>

#include "utilsglobal.h"

namespace RTPTechGroup {
namespace Utils {

//! Класс реализующий FAST PIMPL
/*! Данный класс реализует паттерн FAST PIMPL, который позволяет обеспечить
    бинарную совместимость при изменении реализации.\n
    Пример:\n
        \code
             FastPimpl<string, 32, 8> data("Hello World!");
        \endcode
!*/
template <class T, size_t Size, size_t Alignment>
class UTILSLIB FastPimpl
{
public:
    //! Конструктор класса
    template <class... Args>
    explicit FastPimpl(Args&&... args) {
        new (data()) T(std::forward<Args>(args)...);
    }

    //! Конструктор копирования класса
    FastPimpl(const FastPimpl& rhs) { new (data()) T(*rhs); }

    //! Конструктор перемещения класса
    FastPimpl(FastPimpl&& rhs)      { new (data()) T(std::move(*rhs)); }

    //! Деструктор класса
    ~FastPimpl() noexcept {
        validate<sizeof(T), alignof(T)>();
        data()->~T();
    }

    //! Оператор копирования
    FastPimpl& operator=(FastPimpl& rhs) {
        *data() = *rhs;
        return *this;
    }

    //! Оператор перемещения
    FastPimpl& operator=(FastPimpl&& rhs) {
        *data() = std::move(*rhs);
        return *this;
    }

    //! Оператор доступа к членам класса
    T* operator->() noexcept             { return data(); }

    //! Оператор доступа к константным членам класса
    const T* operator->() const noexcept { return data(); }

    //! Оператор разименования
    T& operator*() noexcept              { return *data(); }

    //! Оператор константного разименования
    const T& operator*() const noexcept  { return *data(); }

private:
    //! Доступ к константным данным
    const T* data() const noexcept {
        return reinterpret_cast<const T*>(&m_data);
    };

    //! Доступ к данным
    T* data() noexcept {
        return reinterpret_cast<T*>(&m_data);
    };

    //! Проверяет размер и выравнивание
    template <std::size_t ActualSize, std::size_t ActualAlignment>
    static void validate() noexcept {
        static_assert(Size == ActualSize, "Size and sizeof(T) mismatch");
        static_assert(Alignment == ActualAlignment, "Alignment and alignof(T) mismatch");
    }

    std::aligned_storage_t<Size, Alignment> m_data;
};

}}

#endif // FASTPIMPL_H
