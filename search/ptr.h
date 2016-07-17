#ifndef PTR_H
#define PTR_H

#include <stdint.h>

static inline void *ptradd(const void *p, unsigned int n)
{
	return (void *)((uintptr_t)p + n);
}

#endif
