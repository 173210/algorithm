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

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../overflow.h"
#include "vertexpath/normal.h"
#include "vertexpath/queue.h"
#include "vertexpath/types.h"
#include "dijkstra.h"
#include "types.h"

int graph_dijkstra(const struct graph *graph)
{
	struct graph_vertexpath_queue *context;
	const struct graph_vertices *vertices;
	const struct graph_vertex *lightest;
	const struct graph_vertexpath *lightest_path;
	const struct graph_vertexpath *target_path;
	struct graph_vertexpath new;
	unsigned long lightest_index;
	unsigned long index;
	unsigned long target;
	long weight;

	vertices = &graph->vertices;

	context = graph_vertexpath_queue_init(vertices);
	if (context == NULL)
		return 1;

	while ((lightest_index = graph_vertexpath_queue_pop(context))
		!= ULONG_MAX)
	{
		lightest = vertices->table + lightest_index;
		lightest_path
			= graph_vertexpath_queue_get(context, lightest_index);

		for (index = 0; index < lightest->edges.number; index++) {
			target = lightest->edges.table[index].target;
			weight = lightest->edges.table[index].weight;
			if (weight < 0) {
				fprintf(stderr, "%lu has a negative weight (%ld), incompatible with Dijkstra's\n",
					index, weight);

				graph_vertexpath_queue_deinit(context);
				return 1;
			}

			if (saddl_overflow(lightest_path->weight, weight,
				&new.weight))
			{
				fprintf(stderr, "ignores a path for %lu longer than %ld\n",
					index, LONG_MAX);

				continue;
			}

			target_path
				= graph_vertexpath_queue_get(context, target);
			if (new.weight < target_path->weight) {
				new.predecessor = lightest_index;
				graph_vertexpath_queue_update(context,
					target, &new);
			}
		}
	}

	graph_vertexpath_queue_print(context);
	graph_vertexpath_queue_deinit(context);
	return 0;
}

int graph_dijkstra_dense(const struct graph *graph)
{
	struct graph_vertexpath_normal *context;
	const struct graph_vertexpath *path;
	struct graph_vertexpath new;
	const struct graph_vertices *vertices;
	struct graph_vertex *lightest;
	unsigned long index;
	unsigned long lightest_index;
	unsigned long target;
	long lightest_weight;
	long weight;

	vertices = &graph->vertices;

	context = graph_vertexpath_normal_init(vertices);
	if (context == NULL)
		return 1;

	while (true) {
		lightest_weight = LONG_MAX;
		for (index = 0; index < vertices->number; index++) {
			path = graph_vertexpath_normal_get(context, index);
			if (vertices->table[index].visited == 0
				&& path->weight < lightest_weight)
			{
				lightest_weight = path->weight;
				lightest_index = index;
			}
		}

		if (lightest_weight >= LONG_MAX)
			break;

		lightest = vertices->table + lightest_index;
		lightest->visited++;

		for (index = 0; index < lightest->edges.number; index++) {
			target = lightest->edges.table[index].target;
			weight = lightest->edges.table[index].weight;
			if (weight < 0) {
				fprintf(stderr, "%lu has a negative weight (%ld), incompatible with Dijkstra's\n",
					index, weight);

				graph_vertexpath_normal_deinit(context);
				return 1;
			}

			if (saddl_overflow(lightest_weight, weight,
				&new.weight))
			{
				fprintf(stderr, "ignores a path for %lu longer than %ld\n",
					index, LONG_MAX);

				continue;
			}

			path = graph_vertexpath_normal_get(context, target);
			if (new.weight < path->weight) {
				new.predecessor = lightest_index;
				graph_vertexpath_normal_update(context,
					target, &new);
			}
		}
	}

	graph_vertexpath_normal_print(context);
	graph_vertexpath_normal_deinit(context);
	return 0;
}
