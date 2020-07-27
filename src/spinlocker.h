#ifndef SPINLOCKER_H
#define SPINLOCKER_H

#include "utilsglobal.h"

namespace RTPTechGroup {
namespace Utils {

//! Класс для удобной и простой блокировки и разблокировки SpinLock и RecursiveSpinLock
template <typename T>
class UTILSLIB SpinLocker
{
public:
    //! Конструктор класса
    SpinLocker(T *spinlock) : m_spinlock(spinlock) {
        if (m_spinlock != nullptr)
            m_spinlock->lock();
    }

    //! Деструктор класса
    ~SpinLocker() {
        if (m_spinlock != nullptr)
            m_spinlock->unlock();
    }

private:
    //! Ссылка на SpinLock
    T *m_spinlock;
};

}}

#endif // SPINLOCKER_H
