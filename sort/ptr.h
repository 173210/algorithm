/*
 * Copyright (C) 2016 173210 <root.3.173210@live.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PTR_H
#define PTR_H

#include <stdint.h>

static inline void *ptradd(const void *p, unsigned int n)
{
	return (void *)((uintptr_t)p + n);
}

static inline void *ptrsub(const void *p, unsigned int n)
{
	return (void *)((uintptr_t)p - n);
}

static inline ptrdiff_t ptrcmp(const void *p, const void *q)
{
	return ((ptrdiff_t)p - (ptrdiff_t)q);
}

#endif
