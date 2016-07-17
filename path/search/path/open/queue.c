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
#include "private.h"
#include "queue.h"

struct context {
	path_evaluate_t evaluate;
	struct path_node *root;
};

struct node {
	struct path_node *child;
	struct path_node *sibling;
	int priority;
};

static void init(void * restrict context_void, path_evaluate_t evaluate)
{
	struct context *context;

	context = context_void;

	context->evaluate = evaluate;
	context->root = NULL;
}

static void push(void * restrict context_void,
	struct path_node * restrict new)
{
	struct context *context;
	struct node *new_open;
	struct node *root_open;

	context = context_void;
	new_open = path_node_get_open(new);
	new_open->priority = context->evaluate(new);

	if (context->root == NULL) {
		new_open->child = NULL;
		new_open->sibling = NULL;
		context->root = new;
	} else {
		root_open = path_node_get_open(context->root);
		if (new_open->priority < root_open->priority) {
			root_open->sibling = NULL;
			new_open->child = context->root;
			new_open->sibling = NULL;
			context->root = new;
		} else {
			new_open->child = NULL;
			new_open->sibling = root_open->child;
			root_open->child = new;
		}
	}
}

static struct path_node *pop(void * restrict context_void)
{
	struct context *context;
	struct path_node *cursor;
	struct path_node *next;
	struct path_node *path;
	struct path_node *popped;
	struct node *cursor_open;
	struct node *popped_open;
	struct node *root_open;
	struct node *open;

	context = context_void;
	popped = context->root;
	if (popped == NULL)
		return NULL;

	popped_open = path_node_get_open(popped);
	context->root = popped_open->child;
	if (context->root != NULL) {
		root_open = path_node_get_open(context->root);
		cursor = root_open->sibling;
		while (cursor != NULL) {
			cursor_open = path_node_get_open(cursor);
			next = cursor_open->sibling;
			if (cursor_open->priority < root_open->priority) {
				open = root_open;
				root_open = cursor_open;
				cursor_open = open;

				path = context->root;
				context->root = cursor;
				cursor = path;
			}

			cursor_open->sibling = root_open->child;
			root_open->child = cursor;
			cursor = next;
		}

		root_open->sibling = NULL;
	}

	return popped;
}

const struct path_open path_open_queue = {
	.init = init,
	.push = push,
	.pop = pop,
	.context = sizeof(struct context),
	.node = sizeof(struct node)
};
