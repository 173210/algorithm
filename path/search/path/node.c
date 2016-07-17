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
#include "list.h"
#include "node.h"
#include "private.h"

static bool zadd_overflow(size_t a, size_t b, size_t *result)
{
#ifdef __GNUC__
	return __builtin_add_overflow(a, b, result);
#else
	if (b > SIZE_MAX - a)
		return true;

	*result = a + b;

	return false;
#endif
}

bool path_node_alloc(const struct path_list * restrict context,
	struct path_node_descriptor * restrict descriptor)
{
	struct path_node *new;
	size_t size;

	if (zadd_overflow(sizeof(*new), context->open->node, &size))
		goto fail_overflow;

	if (zadd_overflow(size, context->state, &size))
		goto fail_overflow;

	new = malloc(size);
	if (new == NULL)
		goto fail_malloc;

	descriptor->node = new;
	descriptor->path = &new->path;
	descriptor->state = path_node_get_state(context, new);

	return false;

fail_overflow:
	fputs("error: too large state size\n", stderr);
	return true;

fail_malloc:
	perror(NULL);
	return true;
}

void path_node_free(struct path_node *node)
{
	free(node);
}
