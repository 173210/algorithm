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
#include "ssearch.h"
#include "ptr.h"

void *ssearch(const void * restrict key, void * restrict base,
	size_t number, size_t size, int (* cmp)(const void *, const void *))
{
	while (number > 0) {
		number--;
		base = ptradd(base, size);
		if (cmp(base, key) == 0)
			return base;
	}

	return NULL;
}
