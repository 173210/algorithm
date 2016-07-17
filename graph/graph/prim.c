#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../overflow.h"
#include "vertexpath/queue.h"
#include "vertexpath/types.h"
#include "prim.h"
#include "types.h"

int graph_prim(const struct graph *graph)
{
	const struct graph_edges *edges;
	const struct graph_edge *edge;
	struct graph_vertexpath_queue *context;
	const struct graph_vertexpath *path;
	struct graph_vertexpath new;
	unsigned long count;

	context = graph_vertexpath_queue_init(&graph->vertices);
	if (context == NULL)
		return 1;

	while ((new.predecessor = graph_vertexpath_queue_pop(context))
		< ULONG_MAX)
	{
		graph->vertices.table[new.predecessor].visited++;

		edges = &graph->vertices.table[new.predecessor].edges;
		edge = edges->table;
		for (count = 0; count < edges->number; count++) {
			path = graph_vertexpath_queue_get(
				context, edge->target);
			if (graph->vertices.table[edge->target].visited <= 0
				&& edge->weight < path->weight)
			{
				new.weight = edge->weight;
				graph_vertexpath_queue_update(context,
					edge->target, &new);
			}

			edge++;
		}
	}

	graph_vertexpath_queue_print(context);
	graph_vertexpath_queue_deinit(context);
	return 0;
}
