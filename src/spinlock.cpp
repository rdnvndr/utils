#include "spinlock.h"

#ifdef _MSC_VER
#include <windows.h>
#endif

namespace RTPTechGroup {
namespace Utils {

void SpinLock::lock() noexcept
{
    for (;;) {
        // Optimistically assume the lock is free on first the try
        if (!m_lock.exchange(true, std::memory_order_acquire)) {
            return;
        }
        // Wait for lock to be released without generating cache misses
        while (m_lock.load(std::memory_order_relaxed)) {
            // Issue X86 PAUSE or ARM YIELD instruction to reduce contention between
            // hyper-threads
            #ifdef _MSC_VER
                _mm_pause();
            #else
                __builtin_ia32_pause();
            #endif
        }
    }
}

bool SpinLock::try_lock() noexcept
{
    // First do a relaxed load to check if lock is free in order to prevent
    // unnecessary cache misses if someone does while(!try_lock())
    return !m_lock.load(std::memory_order_relaxed) &&
           !m_lock.exchange(true, std::memory_order_acquire);
}

void SpinLock::unlock() noexcept
{
    m_lock.store(false, std::memory_order_release);
}

}}
