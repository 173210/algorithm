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

#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <stdbool.h>
#include <stddef.h>

enum game_player_id {
	GAME_PLAYER_NIL = ' ',
	GAME_PLAYER_USR = 'X',
	GAME_PLAYER_COM = 'O'
};

struct game_moves {
	void *table;
	unsigned int number;
};

struct game_state {
	void *state;
	struct game_moves moves;
	enum game_player_id turn;
};

struct game_type;

struct game_player {
	const char *name;
	unsigned int (* move)(struct game_state *state,
		unsigned int (* algorithm)(
			const struct game_type *type,
			struct game_state *state,
			unsigned int predict, int *score));
	enum game_player_id id;
};

struct game_players {
	struct game_player usr;
	struct game_player com;
};

struct game_type {
	bool (* checkover)(struct game_state *state);
	int (* evaluate)(const void *, enum game_player_id);
	void (* move)(struct game_state *, unsigned int, enum game_player_id,
		bool);
	struct game_state *(* init)();
	void (* deinit)(struct game_state *);
	struct game_players players;
	size_t move_size;
};

enum game_player_id game_player_opponent(enum game_player_id player);

#endif
