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

#include "dsearch.h"
#include "types.h"

static void recurse(const struct graph *graph,
	unsigned long index)
{
	const struct graph_edges *edges;
	struct graph_vertex *vertex;
	unsigned long edge;

	vertex = graph->vertices.table + index;

	vertex->visited++;
	if (vertex->visited > 1)
		return;

	edges = &vertex->edges;
	for (edge = 0; edge < edges->number; edge++)
		recurse(graph, edges->table[edge].target);
}

int graph_dsearch(const struct graph *graph)
{
	recurse(graph, 0);
	return 0;
}
