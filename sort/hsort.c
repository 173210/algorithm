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
#include "hsort.h"
#include "memswap.h"
#include "ptr.h"

static void heapify(void * restrict base,
	size_t parent, size_t number, size_t size,
	int (* cmp)(const void *, const void *))
{
	size_t max;
	size_t lower;
	size_t higher;
	void *parentptr;
	void *maxptr;
	void *ptr;

	parentptr = ptradd(base, parent * size);
	max = parent;
	maxptr = parentptr;
	while (1) {
		lower = 2 * parent + 1;
		higher = 2 * parent + 2;

		ptr = ptradd(base, lower * size);
		if (lower < number && cmp(ptr, maxptr) > 0) {
			max = lower;
			maxptr = ptr;
		}

		ptr = ptradd(base, higher * size);
		if (higher < number && cmp(ptr, maxptr) > 0) {
			max = higher;
			maxptr = ptr;
		}

		if (max == parent)
			break;

		memswap(parentptr, maxptr, size);
		parent = max;
		parentptr = maxptr;
	}
}

void hsort(void * restrict base, size_t number, size_t size,
	int (* cmp)(const void *, const void *))
{
	size_t index;

	index = number / 2;
	while (index > 0) {
		index--;
		heapify(base, index, number, size, cmp);
	}

	for (index = number - 1; index > 0; index--) {
		memswap(base, ptradd(base, index * size), size);
		heapify(base, 0, index, size, cmp);
	}
}
