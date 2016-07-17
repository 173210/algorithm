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

#ifndef PATH_LIST_H
#define PATH_LIST_H

#include "node.h"

struct path_list;
struct path_node;
struct path_open;

struct path_list *path_list_init(const struct path_open *open, size_t state,
	path_cmp_t cmp, path_evaluate_t evaluate);

void path_list_deinit(struct path_list *context);

struct path_node *path_list_push(
	struct path_list *context, struct path_node * restrict new);

bool path_list_pop(struct path_list *context,
	struct path_node_descriptor_stored *descriptor);

void *path_list_build_answer(const struct path_list * restrict list,
	const struct path_node *cursor, unsigned int *result_number);

#endif
