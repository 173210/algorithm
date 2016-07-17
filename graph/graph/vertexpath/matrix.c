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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../../overflow.h"
#include "../types.h"
#include "types.h"
#include "matrix.h"

struct graph_vertexpath_matrix {
	unsigned long number;
	struct graph_vertexpath pathes[];
};

struct graph_vertexpath_matrix *graph_vertexpath_matrix_init(
	const struct graph_vertices *vertices)
{
	struct graph_vertexpath_matrix *context;
	const struct graph_edges *edges;
	const struct graph_edge *edge;
	struct graph_vertexpath *row_cursor;
	unsigned long column;
	unsigned long row;
	size_t size;

	if (vertices->number > SIZE_MAX)
		goto fail_overflow;

	if (zmul_overflow(vertices->number, vertices->number, &size))
		goto fail_overflow;

	if (zmul_overflow(size, sizeof(*context->pathes), &size))
		goto fail_overflow;

	if (zadd_overflow(sizeof(*context), size, &size))
		goto fail_overflow;

	context = malloc(size);
	if (context == NULL)
		goto fail_malloc;

	context->number = vertices->number;

	row_cursor = context->pathes;
	for (column = 0; column < context->number; column++) {
		for (row = 0; row < context->number; row++)
			row_cursor[row].weight = LONG_MAX;

		row_cursor[column].weight = 0;
		row_cursor[column].predecessor = column;

		edges = &vertices->table[column].edges;
		for (row = 0; row < edges->number; row++) {
			edge = edges->table + row;
			row_cursor[edge->target].weight = edge->weight;
			row_cursor[edge->target].predecessor = column;
		}

		row_cursor += context->number;
	}

	return context;

fail_overflow:
	fprintf(stderr, "%lu vertices are too many\n", vertices->number);
	return NULL;

fail_malloc:
	perror(NULL);
	return NULL;
}

void graph_vertexpath_matrix_print(
	const struct graph_vertexpath_matrix *context)
{
	const struct graph_vertexpath *cursor;
	unsigned long column;
	unsigned long row;

	printf("predecessor (weight)\n");
	for (column = 0; column < context->number; column++)
		printf("%7lu ", column);

	puts("|");

	cursor = context->pathes;
	for (row = 0; row < context->number; row++) {
		for (column = 0; column < context->number; column++) {
			if (cursor->weight >= LONG_MAX)
				printf("    N/A ");
			else
				printf("%2lu (%2lu) ",
					cursor->predecessor, cursor->weight);

			cursor++;
		}

		printf("| %lu\n", row);
	}
}

#define GET_PATH(context, row, column)	\
	((context)->pathes + (row) * sizeof(*(context)->pathes) + (column))

const struct graph_vertexpath *graph_vertexpath_matrix_get(
	const struct graph_vertexpath_matrix *context,
	unsigned long row, unsigned long column)
{
	return GET_PATH(context, row, column);
}

void graph_vertexpath_matrix_update(struct graph_vertexpath_matrix *context,
	unsigned long row, unsigned long column,
	const struct graph_vertexpath * restrict new)
{
	struct graph_vertexpath *path;

	path = GET_PATH(context, row, column);
	path->weight = new->weight;
	path->predecessor = new->predecessor;
}
