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
#include <stdlib.h>
#include "../overflow.h"
#include "vertexpath/normal.h"
#include "vertexpath/types.h"
#include "bellmanford.h"
#include "types.h"

int graph_bellmanford(const struct graph *graph)
{
	const struct graph_edge *edge;
	struct graph_vertexpath_normal *context;
	struct graph_vertexpath new;
	unsigned long count;
	unsigned long index;
	long source_weight;
	long target_weight;

	context = graph_vertexpath_normal_init(&graph->vertices);
	if (context == NULL)
		return 1;

	for (count = 0; count < graph->vertices.number; count++) {
		for (index = 0; index < graph->edges.number; index++) {
			edge = graph->edges.table + index;
			source_weight = graph_vertexpath_normal_get(
				context, edge->source)->weight;

			if (source_weight >= LONG_MAX)
				continue;

			if (saddl_overflow(source_weight, edge->weight,
				&new.weight))
			{
				fprintf(stderr, "ignoring a longer path for %lu than %ld",
					edge->target, LONG_MAX);

				continue;
			}

			target_weight = graph_vertexpath_normal_get(
				context, edge->target)->weight;

			if (new.weight < target_weight) {
				if (count >= graph->vertices.number) {
					fputs("entered in a negative cycle",
						stderr);

					graph_vertexpath_normal_deinit(context);
					return 1;
				}

				new.predecessor = edge->source;
				graph_vertexpath_normal_update(context,
					edge->target, &new);
			}
		}
	}

	graph_vertexpath_normal_print(context);
	graph_vertexpath_normal_deinit(context);
	return 0;
}
