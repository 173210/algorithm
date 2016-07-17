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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "game/algorithm/alphabeta.h"
#include "game/algorithm/minimax.h"
#include "game/algorithm/negmax.h"
#include "game/game.h"
#include "game/tictactoe.h"
#include "COPYING.h"

static const struct {
	unsigned int (* algorithm)(const struct game_type *type,
		struct game_state *state,
		unsigned int predict, int *score);
	const char *description;
	char key;
} algorithms[] = {
	{ game_algorithm_minimax, "MiniMax", 'm' },
	{ game_algorithm_negmax, "NegMax", 'n' },
	{ game_algorithm_alphabeta, "AlphaBeta", 'a' }
};

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(*array))

int main(int argc, char *argv[])
{
	const struct game_player *turn_players[2];
	unsigned int (* algorithm)(
		const struct game_type *type, struct game_state *state,
		unsigned int predict, int *score);
	struct game_state *state;
	unsigned int index;
	unsigned int move;
	char initiative;

	if (argc == 2 && argv[1][0] == 'l' && argv[1][1] == 0)
		goto license;

	if (argc != 3 || argv[1][1] != 0 || argv[2][1] != 0)
		goto failure_usage;

	initiative = argv[1][0];
	switch (initiative) {
	case GAME_PLAYER_USR:
		turn_players[0] = &game_tictactoe.players.usr;
		turn_players[1] = &game_tictactoe.players.com;
		break;

	case GAME_PLAYER_COM:
		turn_players[0] = &game_tictactoe.players.com;
		turn_players[1] = &game_tictactoe.players.usr;
		break;

	default:
		goto failure_usage;
	}

	index = 0;
	while (algorithms[index].key != argv[2][0]) {
		index++;
		if (index >= ARRAY_SIZE(algorithms))
			goto failure_usage;
	}

	algorithm = algorithms[index].algorithm;

	state = game_tictactoe.init();
	if (state == NULL)
		goto failure;

	while (true) {
		for (index = 0; index < ARRAY_SIZE(turn_players); index++) {
			state->turn = turn_players[index]->id;
			move = turn_players[index]->move(state, algorithm);
			if (move == UINT_MAX)
				goto over;

			game_tictactoe.move(state, move, state->turn, true);

			if (game_tictactoe.checkover(state))
				goto over;
		}
	}

over:
	game_tictactoe.deinit(state);
	return EXIT_SUCCESS;

failure_usage:
	fprintf(stderr, "usage: %s <INITIATIVE> <ALGORITHM>\n"
		"INITIATIVE: %c - player, %c - computer\n"
		"ALGORITHM:\n",
		argc > 0 ? argv[0] : "<EXECUTABLE>",
		GAME_PLAYER_USR, GAME_PLAYER_COM);

	for (index = 0; index < ARRAY_SIZE(algorithms); index++)
		fprintf(stderr, " %c %s\n",
			algorithms[index].key,
			algorithms[index].description);

	fputs("\n"
		"Copyright (C) 2016 173210 <root.3.173210@live.com>\n"
		"This program comes with ABSOLUTELY NO WARRANTY.\n"
		"This is free software, and you are welcome to redistribute it\n"
		"under certain conditions; give `l' for details.\n", stderr);

failure:
	return EXIT_FAILURE;

license:
	print_copying();
	return EXIT_SUCCESS;
}
