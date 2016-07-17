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

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "def.h"

int puzzle8_board_cmp(
	struct puzzle8_state * restrict p, struct puzzle8_state * restrict q)
{
	return memcmp(p->board, q->board, sizeof(p->board));
}

int puzzle8_board_evaluate(const struct puzzle8_state * restrict current,
	const struct puzzle8_state * restrict goal)
{
	unsigned char curc;
	unsigned char curr;
	unsigned char goalc;
	unsigned char goalr;
	char diffc;
	char diffr;
	double score;

	score = 0;
	for (goalr = 0; goalr < PUZZLE8_BOARD_WIDTH; goalr++) {
		for (goalc = 0;
			goalc < PUZZLE8_BOARD_WIDTH;
			goalc++)
		{
			for (curr = 0; curr < PUZZLE8_BOARD_WIDTH; curr++)
			{
				for (curc = 0;
					curc < PUZZLE8_BOARD_WIDTH;
					curc++)
				{
					if (current->board[curr][curc]
						== goal->board[goalr][goalc])
					{
						goto found;
					}
				}
			}

			abort();

found:
			diffc = goalc - curc;
			diffr = goalr - goalr;
			score += sqrt(diffc * diffc + diffr * diffr);
		}
	}

	return score;
}

void puzzle8_board_print(const struct puzzle8_state *state)
{
	unsigned char column;
	unsigned char row;

	row = 0;
	while (true) {
		column = 0;
		while (true) {
			putchar('+');

			if (column >= PUZZLE8_BOARD_WIDTH)
				break;

			putchar('-');
			column++;
		}
		putchar('\n');

		if (row >= PUZZLE8_BOARD_WIDTH)
			break;

		column = 0;
		while (true) {
			putchar('|');

			if (column >= PUZZLE8_BOARD_WIDTH)
				break;

			putchar(state->board[row][column]);
			column++;
		}
		putchar('\n');

		row++;
	}
}
