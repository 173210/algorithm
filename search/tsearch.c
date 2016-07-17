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
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "ptr.h"
#include "tsearch.h"

struct btnode {
	struct btnode *children[2];
	const void *p;
	size_t level;
};

static struct btnode *btinit(const void *p)
{
	struct btnode *node;

	node = malloc(sizeof(*node));
	if (node == NULL) {
		perror(NULL);
		return NULL;
	}

	node->children[0] = NULL;
	node->children[1] = NULL;
	node->p = p;
	node->level = 0;

	return node;
}

static void btfree(struct btnode *p)
{
	unsigned int i;

	for (i = 0; i < sizeof(p->children) / sizeof(*p->children); i++)
		if (p->children[i] != NULL)
			btfree(p->children[i]);

	free(p);
}

static bool btinsert(void * restrict p,
	struct btnode * restrict * restrict cursor,
	int (* cmp)(const void *, const void *))
{
	struct btnode *current;
	unsigned char index;

	current = *cursor;
	if (current == NULL) {
		current = btinit(p);
		if (current == NULL)
			return true;

		*cursor = current;
		return false;
	}

	if (cmp(p, current->p) < 0)
		index = 0;
	else if (cmp(p, current->p) > 0)
		index = 1;
	else
		return false;

	if (btinsert(p, current->children + index, cmp))
		return true;

	if (current->children[0] != NULL
		&& current->level == current->children[0]->level)
	{
		*cursor = current->children[0];
		current->children[0] = (*cursor)->children[1];
		(*cursor)->children[1] = current;
		current = *cursor;
	}

	if (current->children[1] != NULL
		&& current->children[1]->children[1] != NULL
		&& current->level == current->children[1]->children[1]->level)
	{
		*cursor = current->children[1];
		current->children[1] = (*cursor)->children[0];
		(*cursor)->children[0] = current;
		(*cursor)->level++;
		current = *cursor;
	}

	return false;
}

static const void *btsearch(const void * restrict key,
	struct btnode * restrict cursor,
	int (* cmp)(const void *, const void *))
{
	int compared;

	while (cursor != NULL) {
		compared = cmp(cursor->p, key);
		if (compared < 0)
			cursor = cursor->children[0];
		else if (compared > 0)
			cursor = cursor->children[1];
		else
			return cursor->p;
	}

	return NULL;
}

void *tsearch(const void * restrict key, void * restrict base,
	size_t number, size_t size, int (* cmp)(const void *, const void *))
{
	struct btnode *btbase;
	const void *searched;

	btbase = NULL;
	while (number > 0) {
		number--;
		if (btinsert(base, &btbase, cmp)) {
			btfree(btbase);
			return NULL;
		}

		base = ptradd(base, size);
	}

	searched = btsearch(key, btbase, cmp);
	btfree(btbase);
	return (void *)searched;
}
