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

#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include "path/open/fifo.h"
#include "path/open/lifo.h"
#include "path/open/queue.h"
#include "path/node.h"
#include "path/search.h"
#include "puzzle8/board.h"
#include "puzzle8/def.h"
#include "puzzle8/init.h"
#include "puzzle8/iterator.h"
#include "COPYING.h"
#include "array.h"
#include "iterator.h"

static struct {
	const char *name;
	const struct path_open *open;
} algorithms[] = {
	{ "a-star", &path_open_queue },
	{ "breadth-first", &path_open_fifo },
	{ "depth-first", &path_open_lifo }
};

noreturn static void usage(const char *path)
{
	unsigned int index;

	fprintf(stderr, "usage: %s <ALGORITHM>\n"
		"ALGORITHM:\n", path == NULL ? "<EXECUTABLE>" : path);

	for (index = 0; index < ARRAY_SIZE(algorithms); index++)
		fprintf(stderr, "%c %s\n",
			algorithms[index].name[0], algorithms[index].name);

	fputs("\n"
		"Copyright (C) 2016 173210 <root.3.173210@live.com>\n"
		"This program comes with ABSOLUTELY NO WARRANTY.\n"
		"This is free software, and you are welcome to redistribute it\n"
		"under certain conditions; give `l' for details.\n", stderr);

	exit(EXIT_FAILURE);
}

static const struct path_open *find_algorithm(char key)
{
	unsigned int index;

	for (index = 0; index < ARRAY_SIZE(algorithms); index++)
		if (algorithms[index].name[0] == key)
			return algorithms[index].open;

	return NULL;
}

int main(int argc, char *argv[])
{
	const struct path_open *open;
	struct puzzle8_state init;
	struct puzzle8_state goal;
	struct puzzle8_state *cursor;
	struct puzzle8_state *result;
	unsigned int number;

	if (argc != 2)
		usage(argv[0]);

	if (argv[1][0] == 0 || argv[1][1] != 0)
		usage(argv[0]);

	if (argv[1][0] == 'l')
		goto copying;

	open = find_algorithm(argv[1][0]);
	if (open == NULL)
		usage(argv[0]);

	puzzle8_init(&init, "initial state");
	puzzle8_init(&goal, "goal");

	result = path_search(&init, &goal, sizeof(struct puzzle8_state),
		&number, &puzzle8_iterator,
		(path_cmp_t)puzzle8_board_cmp,
		(path_evaluate_t)puzzle8_board_evaluate,
		open);
	if (result == NULL)
		return EXIT_FAILURE;

	cursor = result;
	while (number > 0) {
		puzzle8_board_print(cursor);
		number--;
		cursor++;
	}

	free(result);
	return EXIT_SUCCESS;

copying:
	print_copying();
	return EXIT_SUCCESS;
}
