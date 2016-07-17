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

#ifndef GRAPH_VERTEXPATH_MATRIX_H
#define GRAPH_VERTEXPATH_MATRIX_H

#include <stdlib.h>
#include "../types.h"
#include "types.h"

struct graph_vertexpath_matrix;

struct graph_vertexpath_matrix *graph_vertexpath_matrix_init(
	const struct graph_vertices *vertices);

static inline void graph_vertexpath_matrix_deinit(
	struct graph_vertexpath_matrix *context)
{
	free(context);
}

void graph_vertexpath_matrix_print(
	const struct graph_vertexpath_matrix *context);

const struct graph_vertexpath *graph_vertexpath_matrix_get(
	const struct graph_vertexpath_matrix *context,
	unsigned long row, unsigned long column);

void graph_vertexpath_matrix_update(struct graph_vertexpath_matrix *context,
	unsigned long row, unsigned long column,
	const struct graph_vertexpath * restrict new);

#endif
