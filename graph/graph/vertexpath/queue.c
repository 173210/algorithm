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
#include "queue.h"

struct node {
	unsigned long *reference;
	unsigned long child;
	unsigned long sibling;
	struct graph_vertexpath vertexpath;
};

struct graph_vertexpath_queue {
	unsigned long number;
	unsigned long queue;
	struct node nodes[];
};

struct graph_vertexpath_queue *graph_vertexpath_queue_init(
	const struct graph_vertices *vertices)
{
	struct graph_vertexpath_queue *context;
	unsigned long index;
	size_t size;

	if (vertices->number > SIZE_MAX)
		goto fail_overflow;

	if (zmul_overflow(sizeof(*context->nodes), vertices->number, &size))
		goto fail_overflow;

	if (zadd_overflow(sizeof(*context), size, &size))
		goto fail_overflow;

	context = malloc(sizeof(*context)
		+ vertices->number * sizeof(struct node));
	if (context == NULL)
		goto fail_malloc;

	context->number = vertices->number;
	context->queue = 0;

	context->nodes[0].vertexpath.weight = 0;
	context->nodes[0].reference = &context->queue;
	for (index = 1; index < context->number; index++) {
		context->nodes[index].reference
			= &context->nodes[index - 1].child;
		context->nodes[index].vertexpath.weight = LONG_MAX;
	}

	for (index = 0; index < context->number; index++)
		context->nodes[index].sibling = ULONG_MAX;

	if (context->number > 0)
		for (index = 0; index < context->number - 1; index++)
			context->nodes[index].child = index + 1;

	context->nodes[context->number - 1].child = ULONG_MAX;

	return context;

fail_overflow:
	printf("%lu vertices are too many", vertices->number);
	return NULL;

fail_malloc:
	perror(NULL);
	return NULL;
}

void graph_vertexpath_queue_print(
	const struct graph_vertexpath_queue *context)
{
	unsigned long u;
	unsigned long v;

	for (u = 0; u < context->number; u++) {
		printf("%lu: ", u);
		if (context->nodes[u].vertexpath.weight >= LONG_MAX) {
			puts("no path found");
		} else {
			v = u;
			while (true) {
				printf("%lu", v);

				if (v <= 0)
					break;

				printf(" <- ");
				v = context->nodes[v].vertexpath.predecessor;
			}

			putchar('\n');
		}
	}
}

static unsigned long pop(unsigned long *queue, struct node *nodes)
{
	unsigned long popped;
	unsigned long cursor;
	unsigned long new;
	unsigned long next;
	unsigned long temporary;

	popped = *queue;
	if (popped >= ULONG_MAX)
		return popped;

	new = nodes[popped].child;
	if (new < ULONG_MAX) {
		cursor = nodes[new].sibling;
		while (cursor < ULONG_MAX) {
			next = nodes[cursor].sibling;
			if (nodes[cursor].vertexpath.weight
				< nodes[new].vertexpath.weight)
			{
				temporary = new;
				new = cursor;
				cursor = temporary;
			}

			nodes[cursor].sibling = nodes[new].child;
			if (nodes[new].child < ULONG_MAX)
				nodes[nodes[new].child].reference
					= &nodes[cursor].sibling;

			nodes[new].child = cursor;
			nodes[cursor].reference = &nodes[new].child;

			cursor = next;
		}

		nodes[new].sibling = ULONG_MAX;
		nodes[new].reference = queue;
	}

	*queue = new;
	return popped;
}

unsigned long graph_vertexpath_queue_pop(
	struct graph_vertexpath_queue *context)
{
	return pop(&context->queue, context->nodes);
}

void graph_vertexpath_queue_push(struct graph_vertexpath_queue *context,
	unsigned long new)
{
	struct node *nodes;
	unsigned long *queue;

	nodes = context->nodes;
	queue = &context->queue;

	if (*queue >= ULONG_MAX) {
		nodes[new].child = ULONG_MAX;
		nodes[new].sibling = ULONG_MAX;
		nodes[new].reference = queue;
		*queue = new;
	} else if (nodes[new].vertexpath.weight
		< nodes[*queue].vertexpath.weight)
	{
		nodes[*queue].sibling = ULONG_MAX;
		nodes[*queue].reference = &nodes[new].child;
		nodes[new].child = *queue;
		nodes[new].sibling = ULONG_MAX;
		nodes[new].reference = queue;
		*queue = new;
	} else {
		nodes[new].child = ULONG_MAX;
		nodes[new].sibling = nodes[*queue].child;
		if (nodes[*queue].child < ULONG_MAX)
			nodes[*queue].reference = &nodes[new].sibling;
		nodes[*queue].child = new;
		nodes[new].reference = &nodes[*queue].child;
	}
}

const struct graph_vertexpath *graph_vertexpath_queue_get(
	const struct graph_vertexpath_queue *context, unsigned long index)
{
	return &context->nodes[index].vertexpath;
}

void graph_vertexpath_queue_update(
	struct graph_vertexpath_queue *context,
	unsigned long index, const struct graph_vertexpath * restrict new)
{
	pop(context->nodes[index].reference, context->nodes);
	context->nodes[index].vertexpath.weight = new->weight;
	context->nodes[index].vertexpath.predecessor = new->predecessor;
	graph_vertexpath_queue_push(context, index);
}
