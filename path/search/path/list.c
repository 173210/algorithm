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
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../array.h"
#include "open/private.h"
#include "list.h"
#include "node.h"
#include "private.h"

// AA Tree
static struct path_node *push(const struct path_list *context,
	struct path_node **cursor, struct path_node * restrict new,
	int (* cmp)(const void * restrict, const void * restrict))
{
	struct path_node *old;
	struct path_node *p;
	unsigned char index;
	int compared;

	p = *cursor;
	if (p == NULL) {
		new->children[0] = NULL;
		new->children[1] = NULL;
		new->level = 0;
		*cursor = new;
		return NULL;
	}

	compared = cmp(path_node_get_state(context, new),
		path_node_get_state(context, p));
	if (compared < 0)
		index = 0;
	else if (compared > 0)
		index = 1;
	else
		return p;

	old = push(context, p->children + index, new, cmp);
	if (old != NULL)
		return old;

	if (p->children[0] != NULL
		&& p->level == p->children[0]->level)
	{
		*cursor = p->children[0];
		p->children[0] = (*cursor)->children[1];
		(*cursor)->children[1] = p;
		p = *cursor;
	}

	if (p->children[1] != NULL
		&& p->children[1]->children[1] != NULL
		&& p->level
			== p->children[1]->children[1]->level)
	{
		*cursor = p->children[1];
		p->children[1] = (*cursor)->children[0];
		(*cursor)->children[0] = p;
		(*cursor)->level++;
		p = *cursor;
	}

	return old;
}

static void *get_open(struct path_list *context)
{
	return (void *)(context + 1);
}

struct path_list *path_list_init(const struct path_open *open, size_t state,
	path_cmp_t cmp, path_evaluate_t evaluate)
{
	struct path_list *context;

	context = malloc(sizeof(*context) + open->context);
	if (context == NULL) {
		perror(NULL);
		return NULL;
	}

	context->open = open;
	context->root = NULL;
	context->cmp = cmp;
	context->state = state;

	open->init(get_open(context), evaluate);

	return context;
}

void path_list_deinit(struct path_list *context)
{
	struct path_node *cursor;
	size_t index;

	cursor = context->root;
	if (cursor == NULL)
		return;

	cursor->path.parent = NULL;
	while (cursor != NULL) {
		for (index = 0; ; index++) {
			if (index >= ARRAY_SIZE(cursor->children)) {
				context->root = (void *)cursor->path.parent;
				free(cursor);
				break;
			}

			if (cursor->children[index] != NULL) {
				context->root = cursor->children[index];
				cursor->children[index] = NULL;
				context->root->path.parent = (void *)cursor;
				break;
			}
		}

		cursor = context->root;
	}

	free(context);
}

struct path_node *path_list_push(
	struct path_list *context, struct path_node * restrict new)
{
	struct path_node *old;

	old = push(context, &context->root, new, context->cmp);
	if (old == NULL) {
		context->open->push(get_open(context), new);
		return NULL;
	} else {
		if (new->path.depth < old->path.depth) {
			old->path.depth = new->path.depth;
			old->path.parent = new->path.parent;
		}

		return old;
	}
}

bool path_list_pop(struct path_list *context,
	struct path_node_descriptor_stored *descriptor)
{
	struct path_node *popped;

	popped = context->open->pop(get_open(context));
	if (popped == NULL)
		return true;

	descriptor->node = popped;
	descriptor->path = &popped->path;
	descriptor->state = path_node_get_state(context, popped);

	return false;
}

void *path_list_build_answer(const struct path_list * restrict list,
	const struct path_node *cursor, unsigned int *result_number)
{
	unsigned int number;
	char *answer;
	size_t left;

	number = cursor->path.depth + 1;
	if (result_number != NULL)
		*result_number = number;

	left = number * list->state;
	answer = malloc(left);
	if (answer == NULL) {
		perror(NULL);
		return NULL;
	}

	do {
		left -= list->state;
		memcpy(answer + left, path_node_get_state(list, cursor),
			list->state);

		cursor = cursor->path.parent;
	} while (left > 0);

	return answer;
}
