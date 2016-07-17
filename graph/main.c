#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph/csv.h"
#include "graph/free.h"
#include "graph/print.h"
#include "graph/types.h"
#include "COPYING.h"
#include "arg.h"

int main(int argc, char *argv[])
{
	struct arg_parsed parsed;
	struct graph *graph;

	if (arg_parse(&parsed, argc, argv))
		goto failure_usage;

	if (parsed.algorithm == NULL)
		goto license;

	graph = graph_readcsv(parsed.reverse);
	if (graph == NULL)
		goto failure_graph;

	if (parsed.algorithm(graph))
		goto failure_algorithm;

	graph_print(graph);
	graph_free(graph);
	return EXIT_SUCCESS;

failure_usage:
	arg_printusage(argc > 0 ? argv[0] : "<EXECUTABLE>");
failure_graph:
	return EXIT_FAILURE;

failure_algorithm:
	graph_free(graph);
	return EXIT_FAILURE;

license:
	print_copying();
	return EXIT_SUCCESS;
}
