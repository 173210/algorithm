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

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../overflow.h"
#include "../types.h"
#include "types.h"
#include "normal.h"

struct graph_vertexpath_normal {
	unsigned long number;
	struct graph_vertexpath pathes[];
};

struct graph_vertexpath_normal *graph_vertexpath_normal_init(
	const struct graph_vertices *vertices)
{
	struct graph_vertexpath_normal *context;
	unsigned long index;
	size_t size;

	if (vertices->number > SIZE_MAX)
		goto fail_overflow;

	if (zmul_overflow(vertices->number, sizeof(*context->pathes), &size))
		goto fail_overflow;

	if (zadd_overflow(sizeof(*context), size, &size))
		goto fail_overflow;

	context = malloc(size);
	if (context == NULL)
		goto fail_malloc;

	context->number = vertices->number;
	context->pathes[0].weight = 0;
	for (index = 1; index < context->number; index++)
		context->pathes[index].weight = LONG_MAX;

	return context;

fail_overflow:
	fprintf(stderr, "%lu vertices are too many\n", vertices->number);
fail_malloc:
	return NULL;
}

void graph_vertexpath_normal_print(
	const struct graph_vertexpath_normal *context)
{
	unsigned long u;
	unsigned long v;

	for (u = 0; u < context->number; u++) {
		printf("%lu: ", u);
		if (context->pathes[u].weight >= LONG_MAX) {
			puts("no path found");
		} else {
			v = u;
			while (true) {
				printf("%lu", v);

				if (v <= 0)
					break;

				printf(" <- ");
				v = context->pathes[v].predecessor;
			}

			putchar('\n');
		}
	}
}

const struct graph_vertexpath *graph_vertexpath_normal_get(
	const struct graph_vertexpath_normal *context, unsigned long index)
{
	return context->pathes + index;
}

void graph_vertexpath_normal_update(
	struct graph_vertexpath_normal *context,
	unsigned long index, const struct graph_vertexpath * restrict new)
{
	context->pathes[index].predecessor = new->predecessor;
	context->pathes[index].weight = new->weight;
}
