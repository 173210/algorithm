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

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include "COPYING.h"
#include "db.h"
#include "osearch.h"
#include "ptr.h"
#include "ssearch.h"
#include "tsearch.h"

static const struct {
	void *(* function)(const void * restrict, void * restrict,
		size_t, size_t,
		int (*)(const void *, const void *));
	const char *name;
	char key;
} algorithms[] = {
	{ ssearch, "sequencial search", 's' },
	{ osearch, "binary search with quick sort", 'o' },
	{ tsearch, "binary tree search", 't' }
};

static noreturn void copying()
{
	print_copying();
	exit(EXIT_SUCCESS);
}

static noreturn void usage(const char *executable)
{
	unsigned int index;

	fprintf(stderr, "usage: %s <ALGORITHM> <KEY>\n"
		"\n"
		"ALGORITHM:\n",
		executable == NULL ? "<EXECUTABLE>" : executable);

	for (index = 0;
		index < sizeof(algorithms) / sizeof(*algorithms);
		index++)
	{
		fprintf(stderr, " %c  %s\n",
			algorithms[index].key,
			algorithms[index].name);
	}

	fputs("\n"
		"Copyright (C) 2016 173210 <root.3.173210@live.com>\n"
		"This program comes with ABSOLUTELY NO WARRANTY.\n"
		"This is free software, and you are welcome to redistribute it\n"
		"under certain conditions; give `l' for details.\n", stderr);

	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	struct dbentry *db;
	struct dbentry *found;
	size_t number;
	size_t index;

	if (argc == 2 && argv[1][0] == 'l' && argv[1][1] == 0)
		copying();

	if (argc != 3 || argv[1][0] != 0 || argv[1][1] != 0)
		usage(argv[0]);

	for (index = 0; algorithms[index].key != argv[1][0]; index++)
		if (index >= sizeof(algorithms) / sizeof(*algorithms))
			usage(argv[0]);

	db = dbinit(&number);
	if (db == NULL)
		return EXIT_FAILURE;

	found = algorithms[index].function(
		argv + 2, db, number, sizeof(*db), dbcmp);
	if (found == NULL)
		printf("not found\n");
	else
		printf("found at index %zd\n", found->index);

	dbfree(db, number);
	return EXIT_SUCCESS;
}
