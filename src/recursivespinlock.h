#ifndef RECURSIVESPINLOCK_H
#define RECURSIVESPINLOCK_H

#include <atomic>
#include <thread>

#include "utilsglobal.h"

namespace RTPTechGroup {
namespace Utils {

//! Структура реализующая рекурсивную циклическую блокировку
struct UTILSLIB RecursiveSpinLock
{
    //! Переменная блокировки
    std::atomic<bool>            m_lock    = {0};

    //! Количество рекурсивных блокировок
    int64_t                      m_counter = {0};

    //! Поток владелец
    std::atomic<std::thread::id> m_owner   = {std::thread::id()};

    //! Блокировка RecursiveSpinLock
    void lock() noexcept;

    //! Проверяет блокировку RecursiveSpinLock
    bool try_lock() noexcept;

    //! Разблокировка RecursiveSpinLock
    void unlock() noexcept;
};

}}

#endif // RECURSIVESPINLOCK_H
