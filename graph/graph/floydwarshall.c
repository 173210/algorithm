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
#include <stdint.h>
#include <stdio.h>
#include "../overflow.h"
#include "vertexpath/matrix.h"
#include "vertexpath/types.h"
#include "floydwarshall.h"
#include "types.h"

int graph_floydwarshall(const struct graph *graph)
{
	const struct graph_vertices *vertices;
	const struct graph_vertexpath *first;
	const struct graph_vertexpath *last;
	const struct graph_vertexpath *cursor;
	const struct graph_edge *edge;
	struct graph_vertexpath_matrix *context;
	struct graph_vertexpath new;
	unsigned long number;
	unsigned long u;
	unsigned long v;
	unsigned long w;

	vertices = &graph->vertices;
	number = vertices->number;

	edge = graph->edges.table;
	for (u = 0; u < graph->edges.number; u++) {
		if (edge->weight < 0)
			goto fail_negative;

		edge++;
	}

	context = graph_vertexpath_matrix_init(vertices);
	if (context == NULL)
		goto fail_init;

	for (u = 0; u < number; u++) {
		for (v = 0; v < number; v++) {
			for (w = 0; w < number; w++) {
				cursor = graph_vertexpath_matrix_get(
					context, v, w);
				first = graph_vertexpath_matrix_get(
					context, v, u);
				if (first->weight >= LONG_MAX)
					continue;

				last = graph_vertexpath_matrix_get(
					context, u, w);
				if (last->weight >= LONG_MAX)
					continue;

				if (saddl_overflow(first->weight, last->weight,
					&new.weight))
				{
					fprintf(stderr, "ignoring a path from %lu to %lu longer than %ld\n",
						v, w, LONG_MAX);

					continue;
				}

				if (new.weight < cursor->weight) {
					new.predecessor = last->predecessor;
					graph_vertexpath_matrix_update(
						context, v, w, &new);
				}
			}
		}
	}

	graph_vertexpath_matrix_print(context);
	graph_vertexpath_matrix_deinit(context);
	return 0;

fail_negative:
	fprintf(stderr, "edge %lu,%lu,%ld: negative weights are incompatible with Floyd-Warshall\n"
		"aboring", edge->source, edge->target, edge->weight);

fail_init:
	return 1;
}
