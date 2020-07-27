#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <atomic>

#include "utilsglobal.h"

namespace RTPTechGroup {
namespace Utils {

//! Структура реализующая циклическую блокировку
struct UTILSLIB SpinLock
{
    //! Переменная блокировки
    std::atomic<bool> m_lock = {0};

    //! Блокировка SpinLock
    void lock() noexcept;

    //! Проверяет блокировку SpinLock
    bool try_lock() noexcept;

    //! Разблокировка SpinLock
    void unlock() noexcept;
};

}}

#endif // SPINLOCK_H
