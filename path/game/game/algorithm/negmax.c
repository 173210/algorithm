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
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "negmax.h"
#include "../game.h"

struct context {
	const struct game_type *type;
	struct game_state *state;
	unsigned int remained[];
};

static unsigned int recurse(struct context *context,
	unsigned int number, enum game_player_id turn,
	unsigned int ply, int *score)
{
	struct game_state *state;
	const struct game_type *type;
	unsigned int move;
	unsigned int index;
	unsigned int nextmove;
	unsigned int nextnumber;
	int nextscore;

	state = context->state;
	type = context->type;

	if (ply <= 0 || number <= 0) {
		*score = type->evaluate(state->state, turn);
		return UINT_MAX;
	}

	nextnumber = number - 1;
	move = UINT_MAX;
	*score = -INT_MAX;
	for (index = 0; index < number; index++) {
		nextmove = context->remained[index];

		context->remained[index] = context->remained[nextnumber];
		context->remained[nextnumber] = nextmove;

		type->move(state, nextmove, turn, false);

		recurse(context, nextnumber,
			game_player_opponent(turn),
			ply - 1, &nextscore);

		type->move(state, nextmove, GAME_PLAYER_NIL, false);

		nextscore = nextscore < -INT_MAX ? INT_MAX : -nextscore;
		if (nextscore > *score) {
			move = nextmove;
			*score = nextscore;
		}
	}

	return move;
}

unsigned int game_algorithm_negmax(
	const struct game_type *type, struct game_state *state,
	unsigned int ply, int *score)
{
	struct context *context;
	unsigned int index;
	unsigned int result;

	context = malloc(sizeof(*context)
		+ sizeof(*context->remained) * state->moves.number);
	if (context == NULL) {
		perror(NULL);
		return UINT_MAX;
	}

	for (index = 0; index < state->moves.number; index++)
		context->remained[index] = index;

	context->type = type;
	context->state = state;

	result = recurse(context,
		state->moves.number, state->turn, ply, score);

	free(context);
	return result;
}
