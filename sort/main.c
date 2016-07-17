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

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include "COPYING.h"
#include "hsort.h"
#include "isort.h"
#include "memswap.h"
#include "msort.h"
#include "osort.h"
#include "ptr.h"
#include "ssort.h"

static int mycmp(const void *p, const void *q)
{
	return strcmp(*(char **)p, *(char **)q);
}

/*
Quick sort is not implemented in this program because the pivot decision is
quite a complicated. uses qsort in libc.

Bucket sort is not implemented because hash algorithm can be complicated
for strings.
*/
static const struct {
	void (* function)(void * restrict, size_t, size_t,
		int (*)(const void *, const void *));
	const char *name;
} algorithms[] = {
	{ isort, "insertion" },
	{ ssort, "selection" },
	{ hsort, "heap" },
	{ qsort, "quick" },
	{ msort, "merge" },
	{ osort, "odd-even transposition" }
};

static noreturn void copying()
{
	print_copying();
	exit(EXIT_SUCCESS);
}

static noreturn void usage(const char *executable)
{
	unsigned int index;

	fprintf(stderr, "usage: %s <ALGORITHM>\n"
		"\n"
		"ALGORITHM:\n",
		executable == NULL ? "<EXECUTABLE>" : executable);

	for (index = 0;
		index < sizeof(algorithms) / sizeof(*algorithms);
		index++)
	{
		fprintf(stderr, " %c  %s\n",
			algorithms[index].name[0],
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
	char **p;
	size_t n;
	size_t index;
	int matched;

	if (argc != 2 || argv[1][0] == 0 || argv[1][1] != 0)
		usage(argv[0]);

	if (argv[1][0] == 'l')
		copying();

	for (index = 0; algorithms[index].name[0] != argv[1][0]; index++)
		if (index >= sizeof(algorithms) / sizeof(*algorithms))
			usage(argv[0]);

	p = malloc(sizeof(*p));
	if (p == NULL) {
		perror(NULL);
		return EXIT_FAILURE;
	}

	n = 0;
	while (1) {
		matched = scanf("%ms", p + n);
		if (matched != 1) {
			if (feof(stdin))
				break;

			while (n > 0) {
				n--;
				free(p[n]);
			}

			free(p);

			if (errno)
				perror(NULL);
			else
				fputs("unknown error while receiving string\n",
					stderr);

			return EXIT_FAILURE;
		}

		n++;
		p = realloc(p, (n + 1) * sizeof(*p));
	}

	algorithms[index].function(p, n, sizeof(*p), mycmp);

	for (index = 0; index < n; index++) {
		puts(p[index]);
		free(p[index]);
	}

	free(p);
	return EXIT_SUCCESS;
}
