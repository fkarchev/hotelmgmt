# Makefile
# Makefile for Hotel Management System
#
# Copyright 2015 Ankit Pati <ankitpati@gmail.com>
#
# This program is free software;  you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the  Free Software
# Foundation;  either version 3 of the License,  or (at your option)  any  later
# version.
#
# This  program is distributed in the hope that it will be useful,  but  WITHOUT
# ANY WARRANTY;  without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR  A  PARTICULAR  PURPOSE.  See the GNU  General  Public  License  for  more
# details.
#
# You should have received a copy of the  GNU General Public License along  with
# this program; if not, see <http://www.gnu.org/licenses/>.

CC=cc
CFLAGS=-c
LDFLAGS=
SOURCES=src/getstr.c src/error.c src/conversions.c src/commands/init.c \
        src/commands/list.c src/commands/drop.c src/commands/switch.c \
        src/commands/cd.c src/commands/back.c src/commands/book.c src/main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=hotel-sh

all: $(SOURCES) $(EXECUTABLE) done

$(EXECUTABLE): $(OBJECTS)
	@echo Linking...
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	@echo Compiling...
	$(CC) $(CFLAGS) $< -o $@

clean:
	@echo Cleaning...
	find . -name "*.o" -type f -delete
	find . -name "*.gch" -type f -delete
	strip $(EXECUTABLE)

done:
	@echo Done!

# end of Makefile
