# Copyright (C) 2016 173210 <root.3.173210@live.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CFLAGS := -Og -Wall -Wextra -g -pedantic -pie -std=c11 -fPIC -flto -fno-sanitize-recover -fsanitize=address,undefined -fstack-protector-all -iquote ../copying
LDFLAGS := $(CFLAGS) -fwhole-program

OBJS := graph/bellmanford.o graph/breadth.o graph/csv.o	\
	graph/dijkstra.o graph/dsearch.o graph/floydwarshall.o graph/free.o	\
	graph/prim.o graph/print.o graph/vertexpath/matrix.o	\
	graph/vertexpath/normal.o graph/vertexpath/queue.o arg.o csv.o main.o

a.out: $(OBJS) ../copying/COPYING.o
	$(LINK.o) $^ $(OUTPUT_OPTION)

clean:
	$(RM) a.out $(OBJS)
