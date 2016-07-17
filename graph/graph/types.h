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

#ifndef GRAPH_TYPES_H
#define GRAPH_TYPES_H

enum graph_edge_param {
	GRAPH_EDGE_PARAM_SOURCE,
	GRAPH_EDGE_PARAM_TARGET,
	GRAPH_EDGE_PARAM_WEIGHT
};

struct graph_edge {
	unsigned long source;
	unsigned long target;
	long weight;
};

struct graph_edges {
	struct graph_edge *table;
	unsigned long number;
};

struct graph_vertex {
	struct graph_edges edges;
	unsigned long visited;
};

struct graph_vertices {
	unsigned long number; /* less than ULONG_MAX */
	struct graph_vertex *table;
};

struct graph {
	struct graph_edges edges;
	struct graph_vertices vertices;
};

#endif
