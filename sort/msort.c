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

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memswap.h"
#include "msort.h"
#include "ptr.h"

static void common(void *destination, void *source,
	size_t number, size_t size,
	int (* cmp)(const void *, const void *), void * restrict buffer);

static void recurse(void * restrict destination, void * restrict source,
	size_t number, size_t size,
	int (* cmp)(const void *, const void *), void * restrict buffer)
{
	void * restrict next;

	if (number < 2) {
		memcpy(destination, source, number * size);
	} else if (number == 2) {
		next = ptradd(source, size);
		if (cmp(source, next) < 0) {
			memcpy(destination, source, size);
			memcpy(ptradd(destination, size), next, size);
		} else {
			memcpy(destination, next, size);
			memcpy(ptradd(destination, size), source, size);
		}
	} else {
		common(destination, source, number, size, cmp, buffer);
	}
}

/* number should be greater than 2. */
static void common(void *destination, void *source,
	size_t number, size_t size,
	int (* cmp)(const void *, const void *), void * restrict buffer)
{
	void *lower;
	void *higher;
	void *bufferlower;
	void *bufferhigher;
	void *bufferbottom;
	void **entry;
	size_t partition;

	partition = number / 2;
	lower = source;
	higher = ptradd(source, partition * size);

	bufferlower = buffer;
	bufferhigher = ptradd(buffer, partition * size);
	bufferbottom = ptradd(buffer, number * size);

	recurse(bufferlower, lower, partition, size, cmp, bufferhigher);
	recurse(bufferhigher, higher, number - partition, size, cmp, lower);

	lower = bufferlower;
	higher = bufferhigher;
	while (1) {
		if (lower == bufferhigher) {
			memcpy(destination, higher,
				(uintptr_t)bufferbottom - (uintptr_t)higher);

			break;
		}

		if (higher == bufferbottom) {
			memcpy(destination, lower,
				(uintptr_t)bufferhigher - (uintptr_t)lower);

			break;
		}

		entry = cmp(lower, higher) < 0 ? &lower : &higher;

		memcpy(destination, *entry, size);
		*entry = ptradd(*entry, size);
		destination = ptradd(destination, size);
	}
}

void msort(void * restrict base, size_t number, size_t size,
	int (* cmp)(const void *, const void *))
{
	void * restrict p;

	if (number < 2) {
		return;
	} else if (number == 2) {
		p = ptradd(base, size);
		if (cmp(base, p) > 0)
			memswap(base, p, size);

		return;
	}

	p = malloc(number * size);
	if (p == NULL) {
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	common(base, base, number, size, cmp, p);
	free(p);
}
