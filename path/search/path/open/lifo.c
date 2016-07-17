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

#include <stdbool.h>
#include <stddef.h>
#include "../list.h"
#include "../node.h"
#include "../private.h"
#include "lifo.h"
#include "private.h"

#ifndef __GNUC__
#define __attribute__(attribute)
#endif

static struct path_node **get_next(
	struct path_node * restrict path)
{
	return path_node_get_open(path);
}

static void init(void * restrict context,
	__attribute__((unused)) path_evaluate_t evaluate)
{
	*(struct path_node **)context = NULL;
}

static void push(void * restrict context,
	struct path_node * restrict new)
{
	struct path_node **root;

	root = context;
	*get_next(new) = *root;
	*root = new;
}

static struct path_node *pop(void * restrict context)
{
	struct path_node **root;
	struct path_node *popped;

	root = context;

	popped = *root;
	if (popped != NULL) {
		*root = *get_next(popped);
		*get_next(popped) = false;
	}

	return popped;
}

const struct path_open path_open_lifo = {
	.init = init,
	.push = push,
	.pop = pop,
	.context = sizeof(struct path_node *),
	.node = sizeof(struct path_node *)
};
