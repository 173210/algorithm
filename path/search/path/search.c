#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "list.h"
#include "node.h"
#include "search.h"

void *path_search(const void * restrict init,
	const void * restrict goal, size_t size, unsigned int *number,
	const struct iterator * restrict iterator,
	path_cmp_t cmp, path_evaluate_t evaluate,
	const struct path_open * restrict open)
{
	struct path_node_descriptor_stored popped;
	struct path_node_descriptor new;
	const struct path_node *last;
	struct path_list *list;
	void *iterator_context;
	void *answer;

	list = path_list_init(open, size, cmp, evaluate);
	if (list == NULL)
		goto fail_list;

	if (path_node_alloc(list, &new))
		goto fail_init;

	new.path->depth = 0;
	memcpy(new.state, init, size);

	path_list_push(list, new.node);

	iterator_context = iterator->alloc();
	if (iterator_context == NULL) {
		perror(NULL);
		goto fail_iterator;
	}

	if (path_node_alloc(list, &new))
		goto fail_new;

	while (true) {
		if (path_list_pop(list, &popped)) {
			puts("not found");
			goto fail_notfound;
		}

		if (cmp(popped.state, goal) == 0) {
			last = popped.node;
			break;
		}

		iterator->init(iterator_context);
		while (!iterator->iterate(iterator_context,
			popped.state, new.state))
		{
			new.path->parent = popped.node;
			new.path->depth = popped.path->depth + 1;

			if (path_list_push(list, new.node) == NULL) {
				if (path_node_alloc(list, &new))
					goto fail_new;
			}
		}
	}

	iterator->free(iterator_context);
	path_node_free(new.node);

	answer = path_list_build_answer(list, last, number);
	if (answer == NULL)
		goto fail_solution;

	path_list_deinit(list);

	return answer;

fail_notfound:
	path_node_free(new.node);
fail_new:
	iterator->free(iterator_context);
fail_solution:
fail_iterator:
fail_init:
	path_list_deinit(list);
fail_list:
	return NULL;
}
