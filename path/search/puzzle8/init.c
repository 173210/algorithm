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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "def.h"
#include "init.h"

void puzzle8_init(struct puzzle8_state *state, const char *name)
{
	const unsigned char index_max
		= PUZZLE8_BOARD_WIDTH * PUZZLE8_BOARD_WIDTH;
	unsigned int bit;
	unsigned int index;
	unsigned char row;
	unsigned char column;
	int scanned;
	int separator;
	uint16_t filled;

	printf("setting up %s\n", name);

	filled = 0;
	for (index = 0; index < index_max; index++)
		filled |= 1 << index;
	filled = ~filled;

	memset(state->board, '?', sizeof(state->board));
	for (row = 0; row < PUZZLE8_BOARD_WIDTH; row++) {
		for (column = 0; column < PUZZLE8_BOARD_WIDTH; column++) {
			state->board[row][column] = 'X';
			puzzle8_board_print(state);

			while (true) {
				while (true) {
					printf("enter the value for cell X [1-%u)\n"
						"(' ' for blank, 'r' to apply random values for all remaining cell): ",
						index_max);

					scanned = getchar();
					separator = getchar();

					if (separator != '\n'
						&& separator != EOF)
					{
						continue;
					}

					if (scanned == ' ') {
						state->blank.column = column;
						state->blank.row = row;
						bit = 0;
						break;
					} else if (scanned == 'r') {
						goto random;
					} else if (scanned > '0') {
						index = scanned - '0';
						if (index < index_max) {
							bit = 1 << index;
							break;
						}
					}
				}

				if ((bit & filled) != 0)
					puts("the value is already entered");
				else
					break;
			}

			state->board[row][column] = scanned;
			filled |= bit;
		}
	}

	return;

	for (row = 0; row < PUZZLE8_BOARD_WIDTH; row++) {
		for (column = 0; column < PUZZLE8_BOARD_WIDTH; column++) {
random:
			do {
				index = rand() % index_max;
				bit = 1 << index;
			} while ((bit & filled) != 0);

			if (index == 0) {
				state->board[row][column] = ' ';
				state->blank.column = column;
				state->blank.row = row;
			} else {
				state->board[row][column] = '0' + index;
			}

			filled |= bit;
		}
	}
	puzzle8_board_print(state);

	return;
}
