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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"

int dbcmp(const void *p, const void *q)
{
	return strcmp(((struct dbentry *)p)->p, ((struct dbentry *)q)->p);
}

void dbfree(struct dbentry *db, size_t number)
{
	while (number > 0) {
		number--;
		free(db[number].p);
	}

	free(db);
}

struct dbentry *dbinit(size_t *number)
{
	struct dbentry *db;
	struct dbentry *newdb;
	size_t current;
	int matched;

	db = malloc(sizeof(*db));
	if (db == NULL) {
		perror(NULL);
		return NULL;
	}

	current = 0;
	while (1) {
		db[current].index = current;
		matched = scanf("%ms", &db[current].p);
		if (matched != 1) {
			if (feof(stdin))
				break;

			if (errno)
				perror(NULL);
			else
				fputs("unknown error while receiving string\n",
					stderr);

			dbfree(db, current);
			return NULL;
		}

		current++;
		newdb = realloc(db, (current + 1) * sizeof(*db));
		if (newdb == NULL) {
			perror(NULL);
			dbfree(db, current);
			return NULL;
		}

		db = newdb;
	}

	*number = current;
	return db;
}
