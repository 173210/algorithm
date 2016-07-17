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

#include <stddef.h>
#include "../list.h"
#include "../node.h"
#include "../private.h"
#include "fifo.h"
#include "private.h"

struct context {
	struct path_node *root;
	struct path_node **leaf;
};

#ifndef __GNUC__
#define __attribute__(attribute)
#endif

static struct path_node **get_next(
	struct path_node * restrict node)
{
	return path_node_get_open(node);
}

static void init(void * restrict context_void,
	__attribute__((unused)) path_evaluate_t evaluate)
{
	struct context *context;

	context = context_void;
	context->root = NULL;
	context->leaf = &context->root;
}

void push(void * restrict context_void,
	struct path_node * restrict new)
{
	struct context *context;

	context = context_void;
	*get_next(new) = NULL;
	*context->leaf = new;
	context->leaf = get_next(new);
}

static struct path_node *pop(void * restrict context_void)
{
	struct context *context;
	struct path_node *popped;

	context = context_void;
	popped = context->root;
	if (popped != NULL) {
		context->root = *get_next(popped);
		if (context->root == NULL)
			context->leaf = &context->root;
	}

	return popped;
}

const struct path_open path_open_fifo = {
	.init = init,
	.push = push,
	.pop = pop,
	.context = sizeof(struct context),
	.node = sizeof(struct path_node *)
};
