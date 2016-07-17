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

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../csv.h"
#include "csv.h"
#include "types.h"

struct context {
	struct graph_edge *edge;
	enum graph_edge_param param; /* unsafe */
};

/*
str is unsafe.
This function doesn't validate the indexes of the vertices.
*/
static int receive(void *context_void, const char *str)
{
	struct context *context;
	unsigned long ul;
	long l;
	char *end;

	context = context_void;

	switch (context->param) {
	case GRAPH_EDGE_PARAM_SOURCE:
		ul = strtoul(str, &end, 0);
		if (errno != 0)
			goto failure_std;
		else if (*end != 0)
			goto failure_end;

		context->edge->source = ul;
		break;

	case GRAPH_EDGE_PARAM_TARGET:
		ul = strtoul(str, &end, 0);
		if (errno != 0)
			goto failure_std;
		else if (*end != 0)
			goto failure_end;

		context->edge->target = ul;
		break;

	case GRAPH_EDGE_PARAM_WEIGHT:
		l = strtol(str, &end, 0);
		if (errno != 0)
			goto failure_std;
		else if (*end != 0)
			goto failure_end;

		context->edge->weight = l;
		break;

	default:
		goto failure_toomany;
	}

	context->param++;
	return 0;

failure_std:
	perror(NULL);
	return 1;

failure_end:
	fprintf(stderr, "invalid character: %c\n", *end);
	return 1;

failure_toomany:
	fputs("too many parameter in a edge\n", stderr);
	return 1;
}

static int cmp(const void *p, const void *q)
{
	unsigned long u;
	unsigned long v;

	u = ((struct graph_edge *)p)->source;
	v = ((struct graph_edge *)q)->source;
	if (u < v)
		return -1;
	else if (u > v)
		return 1;
	else
		return 0;
}

struct graph *graph_readcsv(bool reverse)
{
	struct context context;
	struct graph_edges *edges;
	struct graph_vertices *vertices;
	struct graph *graph;
	struct graph_edge *new;
	struct graph_edge *src;
	struct graph_edge *dst;
	unsigned long edge;
	unsigned long top;
	unsigned long vertex;
	enum csv_getrecord_response response;

	graph = malloc(sizeof(*graph));
	if (graph == NULL) {
		perror(NULL);
		goto failure_graph;
	}

	edges = &graph->edges;
	edges->number = 0;
	edges->table = NULL;
	vertices = &graph->vertices;
	while (true) {
		new = realloc(edges->table,
			sizeof(*edges->table) * (edges->number + 1));
		if (new == NULL) {
			perror(NULL);
			goto failure_edges;
		}

		edges->table = new;
		context.edge = edges->table + edges->number;
		context.param = 0;
		response = csv_getrecord(edges->number, receive, &context);
		if (response == CSV_GETRECORD_FAILURE)
			goto failure_edges;

		if (response == CSV_GETRECORD_EOF)
			break;

		edges->number++;
	}

	if (reverse) {
		new = realloc(edges->table,
			sizeof(*edges->table) * edges->number * 2);
		if (new == NULL) {
			perror(NULL);
			goto failure_edges;
		}

		edges->table = new;
		for (edge = 0; edge < edges->number; edge++) {
			src = edges->table + edge;
			dst = edges->table + edges->number + edge;

			dst->source = src->target;
			dst->target = src->source;
			dst->weight = src->weight;
		}

		edges->number *= 2;
	}

	qsort(edges->table, edges->number, sizeof(*edges->table), cmp);

	vertices->number = edges->table[edges->number - 1].source;
	if (!reverse) {
		for (edge = 0; edge < edges->number; edge++) {
			src = edges->table + edge;
			if (src->target > vertices->number) {
				vertices->number = src->target;
			}
		}
	}

	vertices->number++;
	vertices->table = malloc(sizeof(*vertices->table) * vertices->number);
	if (graph->vertices.table == NULL) {
		perror(NULL);
		goto failure_vertices;
	}

	edge = 0;
	for (vertex = 0; vertex < vertices->number; vertex++) {
		top = edge;
		vertices->table[vertex].edges.table = edges->table + top;

		while (edge < edges->number
			&& edges->table[edge].source == vertex)
		{
			edge++;
		}

		vertices->table[vertex].edges.number = edge - top;
		vertices->table[vertex].visited = 0;
	}

	return graph;

failure_vertices:
failure_edges:
	free(edges->table);
	free(graph);
failure_graph:
	return NULL;
}
