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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../iterator.h"
#include "def.h"
#include "iterator.h"

static void *alloc()
{
	void *context;

	context = malloc(sizeof(int));
	if (context == NULL)
		perror(NULL);

	return context;
}

static void init(void *context)
{
	*(int *)context = 0;
}

static bool iterate(void *context, const void *path, void *generated)
{
	const struct puzzle8_state *path8;
	struct puzzle8_state *generated8;
	int *direction;
	unsigned char nextcolumn;
	unsigned char nextrow;

	generated8 = generated;
	path8 = path;
	direction = context;

direct:
	switch (*direction) {
	case 0:
		if (path8->blank.row <= 0) {
			*direction += 3;
			goto direct;
		}

		nextcolumn = path8->blank.column;
		nextrow = path8->blank.row - 1;
		break;

	case 3:
		nextcolumn = path8->blank.column + 1;
		nextrow = path8->blank.row;

		if (nextcolumn >= PUZZLE8_BOARD_WIDTH) {
			*direction += 3;
			goto direct;
		}

		break;

	case 6:
		nextcolumn = path8->blank.column;
		nextrow = path8->blank.row + 1;

		if (nextrow >= PUZZLE8_BOARD_WIDTH) {
			*direction += 3;
			goto direct;
		}

		break;

	case 9:
		if (path8->blank.column <= 0) {
			*direction += 3;
			goto direct;
		}

		nextcolumn = path8->blank.column - 1;
		nextrow = path8->blank.row;
		break;

	default:
		return true;
	}

	memcpy(generated8->board, path8->board, sizeof(generated8->board));
	generated8->board[path8->blank.row][path8->blank.column]
		= generated8->board[nextrow][nextcolumn];
	generated8->board[nextrow][nextcolumn] = ' ';
	generated8->blank.column = nextcolumn;
	generated8->blank.row = nextrow;
	*direction += 3;

	return false;
}

const struct iterator puzzle8_iterator = {
	.alloc = alloc,
	.free = free,
	.init = init,
	.iterate = iterate
};
