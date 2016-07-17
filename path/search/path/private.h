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

#ifndef PATH_PRIVATE_H
#define PATH_PRIVATE_H

#include "open/private.h"
#include "node.h"

struct path_node {
	struct path path;
	struct path_node *children[2];
	unsigned int level;
};

struct path_list {
	const struct path_open *open;
	struct path_node *root;
	path_cmp_t cmp;
	size_t state;
};

#define path_node_get_open(p)	\
	_Generic(p,	\
	struct path_node *: (void *)(p + 1),	\
	const struct path_node *: (const void *)(p + 1))

#define path_node_get_state(context, p)	\
	_Generic(p,	\
	struct path_node *:	\
		(void *)path_node_get_state_strict(context, p),	\
	const struct path_node *:	\
		path_node_get_state_strict(context, p))

static inline const void *path_node_get_state_strict(
	const struct path_list * restrict context,
	const struct path_node *node)
{
	return (char *)(node + 1) + context->open->node;
}

#endif
