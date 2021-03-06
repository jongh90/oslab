/*
 *  arch/arm/include/asm/atomic.h
 *
 *  Copyright (C) 1996 Russell King.
 *  Copyright (C) 2002 Deep Blue Solutions Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ARCH_ARM_ARM32_ATOMIC__
#define __ARCH_ARM_ARM32_ATOMIC__

/*
 * ARMv6 UP and SMP safe atomic ops.  We use load exclusive and
 * store exclusive to ensure that these are atomic.  We may loop
 * to ensure that the update happens.
 */
static inline void atomic_add(int i, atomic_t *v)
{
        unsigned long tmp;
        int result;

        __asm__ __volatile__("@ atomic_add\n"
"1:     ldrex   %0, [%3]\n"
"       add     %0, %0, %4\n"
"       strex   %1, %0, [%3]\n"
"       teq     %1, #0\n"
"       bne     1b"
        : "=&r" (result), "=&r" (tmp), "+Qo" (v->counter)
        : "r" (&v->counter), "Ir" (i)
        : "cc");
}

static inline int atomic_add_return(int i, atomic_t *v)
{
        unsigned long tmp;
        int result;

        smp_mb();

        __asm__ __volatile__("@ atomic_add_return\n"
"1:     ldrex   %0, [%3]\n"
"       add     %0, %0, %4\n"
"       strex   %1, %0, [%3]\n"
"       teq     %1, #0\n"
"       bne     1b"
        : "=&r" (result), "=&r" (tmp), "+Qo" (v->counter)
        : "r" (&v->counter), "Ir" (i)
        : "cc");

        smp_mb();

        return result;
}

static inline void atomic_sub(int i, atomic_t *v)
{
        unsigned long tmp;
        int result;

        __asm__ __volatile__("@ atomic_sub\n"
"1:     ldrex   %0, [%3]\n"
"       sub     %0, %0, %4\n"
"       strex   %1, %0, [%3]\n"
"       teq     %1, #0\n"
"       bne     1b"
        : "=&r" (result), "=&r" (tmp), "+Qo" (v->counter)
        : "r" (&v->counter), "Ir" (i)
        : "cc");
}

static inline int atomic_sub_return(int i, atomic_t *v)
{
        unsigned long tmp;
        int result;

        smp_mb();

        __asm__ __volatile__("@ atomic_sub_return\n"
"1:     ldrex   %0, [%3]\n"
"       sub     %0, %0, %4\n"
"       strex   %1, %0, [%3]\n"
"       teq     %1, #0\n"
"       bne     1b"
        : "=&r" (result), "=&r" (tmp), "+Qo" (v->counter)
        : "r" (&v->counter), "Ir" (i)
        : "cc");

        smp_mb();

        return result;
}

static inline int atomic_cmpxchg(atomic_t *ptr, int old, int new)
{
        unsigned long oldval, res;

        smp_mb();

        do {
                __asm__ __volatile__("@ atomic_cmpxchg\n"
                "ldrex  %1, [%3]\n"
                "mov    %0, #0\n"
                "teq    %1, %4\n"
                "strexeq %0, %5, [%3]\n"
                    : "=&r" (res), "=&r" (oldval), "+Qo" (ptr->counter)
                    : "r" (&ptr->counter), "Ir" (old), "r" (new)
                    : "cc");
        } while (res);

        smp_mb();

        return oldval;
}

static inline void atomic_clear_mask(unsigned long mask, unsigned long *addr)
{
        unsigned long tmp, tmp2;

        __asm__ __volatile__("@ atomic_clear_mask\n"
"1:     ldrex   %0, [%3]\n"
"       bic     %0, %0, %4\n"
"       strex   %1, %0, [%3]\n"
"       teq     %1, #0\n"
"       bne     1b"
        : "=&r" (tmp), "=&r" (tmp2), "+Qo" (*addr)
        : "r" (addr), "Ir" (mask)
        : "cc");
}

#define atomic_inc(v)           atomic_add(1, v)
#define atomic_dec(v)           atomic_sub(1, v)

#define atomic_inc_and_test(v)  (atomic_add_return(1, v) == 0)
#define atomic_dec_and_test(v)  (atomic_sub_return(1, v) == 0)
#define atomic_inc_return(v)    (atomic_add_return(1, v))
#define atomic_dec_return(v)    (atomic_sub_return(1, v))
#define atomic_sub_and_test(i, v) (atomic_sub_return(i, v) == 0)

#define atomic_add_negative(i,v) (atomic_add_return(i, v) < 0)

#endif /* __ARCH_ARM_ARM32_ATOMIC__ */
/*
 * Local variables:
 * mode: C
 * c-file-style: "BSD"
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
