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

#ifndef ARG_H
#define ARG_H

#include <stdbool.h>
#include "graph/types.h"

struct arg_parsed {
	bool reverse;
	int (* algorithm)(const struct graph *);
	unsigned long start;
	unsigned long end;
};

bool arg_parse(struct arg_parsed *parsed, int argc, char *argv[]);
void arg_printusage(const char *executable);

#endif
