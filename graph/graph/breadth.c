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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../overflow.h"
#include "breadth.h"
#include "types.h"

int graph_breadth(const struct graph *graph)
{
	const struct graph_vertices *vertices;
	const struct graph_edges *edges;
	unsigned long *next_table;
	unsigned long next;
	unsigned long index;
	unsigned long edge;
	unsigned long number;
	size_t size;

	vertices = &graph->vertices;
	if (vertices->number > SIZE_MAX
		|| zmul_overflow(sizeof(*next_table), vertices->number, &size))
	{
		fprintf(stderr, "%lu vertices are too many", vertices->number);
		return 1;
	}

	next_table = malloc(size);
	if (next_table == NULL) {
		perror(NULL);
		return 1;
	}

	next_table[0] = 0;
	vertices->table[0].visited++;
	number = 1;
	while (number > 0) {
		index = number;
		while (index > 0) {
			index--;
			edges = &vertices->table[next_table[index]].edges;

			number--;
			next_table[index] = next_table[number];

			for (edge = 0; edge < edges->number; edge++) {
				next = edges->table[edge].target;
				vertices->table[next].visited++;

				if (vertices->table[next].visited > 1)
					continue;

				next_table[number] = next;
				number++;
			}
		}
	}

	free(next_table);
	return 0;
}
