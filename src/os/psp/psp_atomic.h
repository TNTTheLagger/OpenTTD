#ifndef PSP_ATOMIC_H
#define PSP_ATOMIC_H

#ifdef PSP

#include <pspkernel.h>

/* Simple atomic operations for PSP using interrupts */
template <typename T>
static inline T __atomic_load_n(const T *ptr, int memorder)
{
    int intr = sceKernelCpuSuspendIntr();
    T value = *ptr;
    sceKernelCpuResumeIntr(intr);
    return value;
}

template <typename T>
static inline void __atomic_store_n(T *ptr, T value, int memorder)
{
    int intr = sceKernelCpuSuspendIntr();
    *ptr = value;
    sceKernelCpuResumeIntr(intr);
}

template <typename T>
static inline T __atomic_fetch_add(T *ptr, T value, int memorder)
{
    int intr = sceKernelCpuSuspendIntr();
    T old_value = *ptr;
    *ptr += value;
    sceKernelCpuResumeIntr(intr);
    return old_value;
}

template <typename T>
static inline T __atomic_fetch_sub(T *ptr, T value, int memorder)
{
    int intr = sceKernelCpuSuspendIntr();
    T old_value = *ptr;
    *ptr -= value;
    sceKernelCpuResumeIntr(intr);
    return old_value;
}

template <typename T>
static inline bool __atomic_compare_exchange_n(T *ptr, T *expected, T desired, bool weak, int success_memorder, int failure_memorder)
{
    int intr = sceKernelCpuSuspendIntr();
    bool success = (*ptr == *expected);
    if (success)
    {
        *ptr = desired;
    }
    else
    {
        *expected = *ptr;
    }
    sceKernelCpuResumeIntr(intr);
    return success;
}

#endif /* PSP */

#endif /* PSP_ATOMIC_H */
