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
#include <stdio.h>
#include "graph/bellmanford.h"
#include "graph/breadth.h"
#include "graph/dijkstra.h"
#include "graph/dsearch.h"
#include "graph/floydwarshall.h"
#include "graph/prim.h"
#include "graph/types.h"
#include "arg.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))

static const struct {
	const char *description;
	char arg;
} arg_options[] = {
	{ "make a reversed edge for each ones.", 'r' }
};

static const struct {
	int (* function)(const struct graph *);
	const char *description;
	char arg;
} arg_algorithms[] = {
	{ graph_dsearch, "Depth-First Search", 's' },
	{ graph_breadth, "Breadth-First Search", 't' },
	{ graph_dijkstra, "Dijkstra's", 'd' },
	{ graph_dijkstra_dense, "Dijkstra's for Dense Graph", 'e' },
	{ graph_bellmanford, "Bellman-Ford", 'b' },
	{ graph_floydwarshall, "Floyd-Warshall", 'f' },
	{ graph_prim, "Prim", 'p' },
	{ NULL, "Show license", 'l' }
};

static bool arg_algorithms_search(char arg,
	int (** result)(const struct graph *))
{
	size_t index;

	for (index = 0; index < ARRAY_SIZE(arg_algorithms); index++) {
		if (arg_algorithms[index].arg == arg) {
			*result = arg_algorithms[index].function;
			return false;
		}
	}

	return true;
}

bool arg_parse(struct arg_parsed *parsed, int argc, char *argv[])
{
	bool found_algorithm;
	int index;

	argc--;
	argv++;
	parsed->reverse = false;
	parsed->algorithm = NULL;
	parsed->start = 0;
	parsed->end = ULONG_MAX;
	found_algorithm = false;
	for (index = 0; index < argc; index++) {
		if (argv[index][0] == '-') {
			if (argv[index][1] == 'r' && argv[index][2] == 0)
				parsed->reverse = true;
			else
				return true;
		} else {
			if (found_algorithm || argv[index][1] != 0)
				return true;

			if (arg_algorithms_search(
				argv[index][0], &parsed->algorithm))
			{
				return true;
			}

			found_algorithm = true;
		}
	}

	return !found_algorithm;
}

void arg_printusage(const char *executable)
{
	size_t index;

	fprintf(stderr, "usage: %s [OPTIONS] <ALGORITHM>\n"
		"OPTIONS:\n", executable);

	for (index = 0; index < ARRAY_SIZE(arg_options); index++)
		fprintf(stderr, " -%c %s\n",
			arg_options[index].arg,
			arg_options[index].description);

	fputs("ALGORITHM:\n", stderr);
	for (index = 0; index < ARRAY_SIZE(arg_algorithms); index++)
		fprintf(stderr, "  %c %s\n",
			arg_algorithms[index].arg,
			arg_algorithms[index].description);

	fputs("\n"
		"Copyright (C) 2016 173210 <root.3.173210@live.com>\n"
		"This program comes with ABSOLUTELY NO WARRANTY.\n"
		"This is free software, and you are welcome to redistribute it\n"
		"under certain conditions; give `l' for details.\n", stderr);
}
