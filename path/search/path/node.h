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

#ifndef PATH_NODE_H
#define PATH_NODE_H

#include <stdbool.h>

struct path_node_descriptor {
	struct path_node *node;
	struct path *path;
	void *state;
};

struct path_node_descriptor_stored {
	struct path_node *node;
	struct path *path;
	const void *state;
};

typedef int (* path_cmp_t)(const void * restrict, const void * restrict);
typedef unsigned int (* path_evaluate_t)(const void *);

struct path {
	struct path_node *parent;
	unsigned int depth;
};

struct path_list;

bool path_node_alloc(const struct path_list * restrict context,
	struct path_node_descriptor * restrict descriptor);

void path_node_free(struct path_node *node);

#endif
