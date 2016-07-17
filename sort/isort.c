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
#include "isort.h"
#include "ptr.h"

void isort(void * restrict base, size_t number, size_t size,
	int (* cmp)(const void *, const void *))
{
	char temporary;
	char *bottom;
	char *upper;
	char *lower;
	char *cursor;
	char *previous;
	size_t i;

	if (number < 2)
		return;

	bottom = ptradd(base, number * size);
	upper = base;
	while (1) {
		upper = ptradd(upper, size);
		if (upper == bottom)
			break;

		for (lower = upper; lower != base; lower = previous) {
			previous = ptrsub(lower, size);
			if (cmp(previous, upper) <= 0)
				break;
		}

		for (i = 0; i < size; i++) {
			temporary = upper[i];

			cursor = upper;
			while (cursor != lower) {
				previous = ptrsub(cursor, size);
				cursor[i] = previous[i];
				cursor = previous;
			}

			cursor[i] = temporary;
		}
	}
}
