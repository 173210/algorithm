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

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "csv.h"

static void csv_getfield(char *buffer, size_t size, int *character)
{
	char *last;

	if (size <= 0)
		return;

	last = buffer + size - 1;
	while (true) {
		switch (*character) {
		case EOF:
		case ',':
		case '\r':
			goto end;

		case '"':
			while ((*character = getchar()) == '"') {
				if (*character == EOF || buffer == last)
					goto end;

				*buffer = *character;
				buffer++;
			}
			break;

		default:
			if (buffer == last)
				goto end;

			*buffer = *character;
			buffer++;
			*character = getchar();
			break;
		}
	}

end:
	*buffer = 0;
}

enum csv_getrecord_response csv_getrecord(unsigned long line,
	int (* receive)(void *, const char *), void *context)
{
	char field[8];
	int character;

	character = getchar();
	if (character == EOF)
		return CSV_GETRECORD_EOF;

	while (true) {
		csv_getfield(field, sizeof(field), &character);
		if (field[0] != 0) {
			if (receive(context, field) != 0)
				goto failure;
		}

		if (character == '\r') {
			if (getchar() != '\n')
				goto failure_lf;

			break;
		}

		if (character != ',')
			goto failure_character;

		character = getchar();
	}

	return CSV_GETRECORD_SUCCESS;

failure_character:
	fprintf(stderr, "%lu: invalid character: ", line);
	if (isprint(character))
		putc(character, stderr);
	else
		fprintf(stderr, "\\%d", character);

	putc('\n', stderr);
	goto failure;

failure_lf:
	fprintf(stderr, "%lu: LF doesn't follow CR\n", line);
	goto failure;

failure:
	return CSV_GETRECORD_FAILURE;
}
