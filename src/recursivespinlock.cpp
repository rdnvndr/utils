#include "recursivespinlock.h"

#ifdef _MSC_VER
#include <windows.h>
#endif

namespace RTPTechGroup {
namespace Utils {

void RecursiveSpinLock::lock() noexcept
{
    for (;;) {
        if (!m_lock.exchange(true, std::memory_order_acquire))
        {
            m_owner.store(std::this_thread::get_id(), std::memory_order_release);
            ++m_counter;
            return;
        } else if (m_owner.load(std::memory_order_acquire) == std::this_thread::get_id()) {
            ++m_counter;
            return;
        } else while (m_lock.load(std::memory_order_relaxed)) {
            #ifdef _MSC_VER
                _mm_pause();
            #else
                __builtin_ia32_pause();
            #endif
        }
    }
}

bool RecursiveSpinLock::try_lock() noexcept
{
    if (!m_lock.load(std::memory_order_relaxed) &&
        !m_lock.exchange(true, std::memory_order_acquire))
    {
        m_owner.store(std::this_thread::get_id(), std::memory_order_release);
    } else {
        if (m_owner.load(std::memory_order_acquire) != std::this_thread::get_id())
            return false;
    }
    ++m_counter;
    return true;
}

void RecursiveSpinLock::unlock() noexcept
{
    if (--m_counter == 0) {
        m_owner.store(std::this_thread::get_id(), std::memory_order_release);
        m_lock.store(false, std::memory_order_release);
    }
}

}}
