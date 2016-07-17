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

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "tictactoe.h"

#define GAME_TICTACTOE_WIDTH 3

typedef enum game_player_id game_tictactoe_board_row_t[GAME_TICTACTOE_WIDTH];

#define GAME_TICTACTOE_BOARD_SIZE	\
	(sizeof(game_tictactoe_board_row_t) * GAME_TICTACTOE_WIDTH)

#define GAME_TICTACTOE_BOARD_DECL(name)	\
	game_tictactoe_board_row_t name[GAME_TICTACTOE_WIDTH]

#define GAME_TICTACTOE_BOARD_ARG(name)	\
	game_tictactoe_board_row_t name[static GAME_TICTACTOE_WIDTH]

#define GAME_TICTACTOE_BOARD_PTR(name)	\
	game_tictactoe_board_row_t *name

#define GAME_TICTACTOE_MOVES_DECL(name)	\
	struct game_tictactoe_cell	\
		name[GAME_TICTACTOE_WIDTH * GAME_TICTACTOE_WIDTH]

struct game_tictactoe_cell {
	unsigned int column;
	unsigned int row;
};

static void monitor(GAME_TICTACTOE_BOARD_ARG(board), struct game_moves *moves)
{
	struct game_tictactoe_cell *table;
	unsigned int number;
	unsigned int column;
	unsigned int row;

	printf("player: %c, computer: %c\n",
		GAME_PLAYER_USR, GAME_PLAYER_COM);

	if (moves != NULL)
		table = moves->table;

	number = 0;
	for (column = 0; column < GAME_TICTACTOE_WIDTH; column++) {
		for (row = 0; row < GAME_TICTACTOE_WIDTH; row++)
			printf("+-");

		printf("+\n");

		for (row = 0; row < GAME_TICTACTOE_WIDTH; row++) {
			putchar('|');

			if (moves != NULL
				&& board[column][row] == GAME_PLAYER_NIL)
			{
				table[number].column = column;
				table[number].row = row;
				number++;
				printf("%d", number);
			} else {
				putchar(board[column][row]);
			}
		}

		printf("|\n");
	}

	for (row = 0; row < GAME_TICTACTOE_WIDTH; row++)
		printf("+-");

	printf("+\n");
	if (moves != NULL)
		moves->number = number;
}

static unsigned int move_usr(struct game_state *state,
	unsigned int (* function)(
		const struct game_type *type,
		struct game_state *state,
		unsigned int predict, int *score) __attribute__((unused)))
{
	unsigned int id;
	int character;

	monitor(state->state, &state->moves);
	while (true) {
		printf("enter your move (no input: resign): ");
		character = getchar();
		if (character == '\n') {
			printf("%s resigned.\n",
				game_tictactoe.players.usr.name);

			return UINT_MAX;
		}

		if (character < '1')
			goto out;

		id = character - '1';
		if (id >= state->moves.number)
			goto out;

		character = getchar();
		if (character == '\n' || character == EOF)
			return id;

out:
		puts("out of range");
	}
}

static unsigned int move_com(struct game_state *state,
	unsigned int (* function)(
		const struct game_type *type,
		struct game_state *state,
		unsigned int predict, int *score))
{
	unsigned int id;
	int score;

	id = function(&game_tictactoe, state, 8, &score);
	if (id >= UINT_MAX) {
		monitor(state->state, NULL);
		printf("%s resigned.\n",
			game_tictactoe.players.com.name);

		return UINT_MAX;
	}

	return id;
}

static void move(struct game_state *state, unsigned int cell,
	enum game_player_id player_id, bool finalize)
{
	GAME_TICTACTOE_BOARD_PTR(board);
	struct game_tictactoe_cell *table;
	const struct game_player *player;

	board = state->state;
	table = state->moves.table;

	board[table[cell].column][table[cell].row] = player_id;

	if (finalize) {
		switch (player_id) {
		case GAME_PLAYER_COM:
			player = &game_tictactoe.players.com;
			break;

		case GAME_PLAYER_USR:
			player = &game_tictactoe.players.usr;
			break;

		default:
			assert(false);
			break;
		}

		printf("%s took (%d, %d)\n", player->name,
			table[cell].row, table[cell].column);

		state->moves.number--;
		table[cell].column = table[state->moves.number].column;
		table[cell].row = table[state->moves.number].row;
	}
}

static int evaluate(const void *state, enum game_player_id player)
{
	/*
	"If the specification of an array type includes any type qualifiers, the
	 element type is so-qualified, not the array type." (ISO/IEC 9899:201x)
	That's why union is necessary to apply const qualifier.
	*/
	const union {
		GAME_TICTACTOE_BOARD_DECL(board);
	} *container;
	unsigned int column;
	unsigned int row;

	container = state;
	for (column = 0; column < GAME_TICTACTOE_WIDTH; column++) {
		row = 0;
		while (container->board[column][row] == player) {
			row++;
			if (row >= GAME_TICTACTOE_WIDTH)
				return 1;
		}
	}

	for (row = 0; row < GAME_TICTACTOE_WIDTH; row++) {
		column = 0;
		while (container->board[column][row] == player) {
			column++;
			if (column >= GAME_TICTACTOE_WIDTH)
				return 1;
		}
	}

	row = 0;
	while (container->board[row][row] == player) {
		row++;
		if (row >= GAME_TICTACTOE_WIDTH)
			return 1;
	}

	row = 0;
	while (container->board[GAME_TICTACTOE_WIDTH - row - 1][row] == player)
	{
		row++;
		if (row >= GAME_TICTACTOE_WIDTH)
			return 1;
	}

	return 0;
}

static bool checkover(struct game_state *state)
{
	GAME_TICTACTOE_BOARD_PTR(board);
	enum game_player_id winner;
	unsigned int column;
	unsigned int row;

	board = state->state;

	for (column = 0; column < GAME_TICTACTOE_WIDTH; column++) {
		winner = board[column][0];
		if (winner == GAME_PLAYER_NIL)
			continue;

		row = 1;
		while (board[column][row] == winner) {
			row++;
			if (row >= GAME_TICTACTOE_WIDTH)
				goto over;
		}
	}

	for (row = 0; row < GAME_TICTACTOE_WIDTH; row++) {
		winner = board[0][row];
		if (winner == GAME_PLAYER_NIL)
			continue;

		column = 1;
		while (board[column][row] == winner) {
			column++;
			if (column >= GAME_TICTACTOE_WIDTH)
				goto over;
		}
	}

	winner = board[0][0];
	if (winner != GAME_PLAYER_NIL) {
		row = 1;
		while (board[row][row] == winner) {
			row++;
			if (row >= GAME_TICTACTOE_WIDTH)
				goto over;
		}
	}

#if GAME_TICTACTOE_WIDTH < 2
#error "GAME_TICTACTOE_WIDTH is too small."
#endif
	winner = board[GAME_TICTACTOE_WIDTH - 1][0];
	if (winner != GAME_PLAYER_NIL) {
		row = 1;
		while (board[GAME_TICTACTOE_WIDTH - row - 1][row] == winner) {
			row++;
			if (row >= GAME_TICTACTOE_WIDTH)
				goto over;
		}
	}

	if (state->moves.number <= 0)
		goto drawn;

	return false;

over:
	monitor(board, NULL);
	printf("winner is %s!\n",
		winner == GAME_PLAYER_USR ? "player" : "computer");

	return true;

drawn:
	monitor(board, NULL);
	puts("the game is drawn.");
	return true;
}

static struct game_state *init()
{
	struct {
		struct game_state state;
		GAME_TICTACTOE_BOARD_DECL(board);
		GAME_TICTACTOE_MOVES_DECL(moves);
	} *buffer;
	unsigned int column;
	unsigned int row;

	buffer = malloc(sizeof(*buffer));
	if (buffer == NULL) {
		perror(NULL);
		return NULL;
	}

	buffer->state.state = buffer->board;
	buffer->state.moves.table = buffer->moves;
	buffer->state.moves.number = 0;
	for (column = 0; column < GAME_TICTACTOE_WIDTH; column++) {
		for (row = 0; row < GAME_TICTACTOE_WIDTH; row++) {
			buffer->board[column][row] = GAME_PLAYER_NIL;

			buffer->moves[buffer->state.moves.number].column
				= column;

			buffer->moves[buffer->state.moves.number].row
				= row;

			buffer->state.moves.number++;
		}
	}

	return &buffer->state;
}

static void deinit(struct game_state *state)
{
	free(state);
}

const struct game_type game_tictactoe = {
	.players = {
		.usr = {
			"player", move_usr, GAME_PLAYER_USR
		},
		.com = {
			"computer", move_com, GAME_PLAYER_COM
		}
	},
	.move_size = sizeof(struct game_tictactoe_cell),
	.checkover = checkover,
	.evaluate = evaluate,
	.move = move,
	.init = init,
	.deinit = deinit
};
