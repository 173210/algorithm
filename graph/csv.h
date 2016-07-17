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

#ifndef CSV_H
#define CSV_H

enum csv_getrecord_response {
	CSV_GETRECORD_SUCCESS,
	CSV_GETRECORD_FAILURE,
	CSV_GETRECORD_EOF
};

enum csv_getrecord_response csv_getrecord(unsigned long line,
	int (* receive)(void *, const char *), void *context);

#endif
