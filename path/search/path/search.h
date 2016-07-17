#ifndef PATH_SEARCH_H
#define PATH_SEARCH_H

#include "../iterator.h"
#include "node.h"

#include <stddef.h>

void *path_search(const void * restrict init,
	const void * restrict goal, size_t size, unsigned int *number,
	const struct iterator * restrict iterator,
	path_cmp_t cmp, path_evaluate_t evaluate,
	const struct path_open * restrict open);

#endif
