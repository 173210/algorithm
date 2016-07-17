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

#ifndef OVERFLOW_H
#define OVERFLOW_H

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

static inline bool saddl_overflow(long l, long m, long *response)
{
#ifdef __GNUC__
	return __builtin_saddl_overflow(l, m, response);
#else
	if (LONG_MAX - l < m)
		return true;

	*response = l + m;
	return false;
#endif
}

static inline bool zadd_overflow(size_t a, size_t b, size_t *response)
{
#ifdef __GNUC__
	return __builtin_add_overflow(a, b, response);
#else
	if (SIZE_MAX - a < b)
		return true;

	*response = a + b;
	return false;
#endif
}

static inline bool zmul_overflow(size_t a, size_t b, size_t *response)
{
#ifdef __GNUC__
	return __builtin_mul_overflow(a, b, response);
#else
	if (SIZE_MAX / a < b)
		return true;

	*response = a * b;
	return false;
#endif
}

#endif
