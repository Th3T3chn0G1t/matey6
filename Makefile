SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard include/m6/*.h)
OBJECTS = $(SOURCES:.c=.o)

OUT = src/matey6

CFLAGS += -Iinclude -include m6/common.h
CFLAGS += -std=c89 -Wall -Wextra -Werror -ansi -pedantic -pedantic-errors
 # POSIX.1-2001 Needed for `getopt`
CFLAGS += -D_POSIX_C_SOURCE=200112L
CFLAGS += -DM6_VERSION=\"0.0.1\" $(M6_CFLAGS)

PREFIX ?= /usr/local

.DEFAULT_GOAL := all
.PHONY: all
all: $(OUT)

$(OUT): $(OBJECTS)

$(OBJECTS): $(HEADERS)

.PHONY: install
install: $(OUT)
	install -sS -m777 $(OUT) $(PREFIX)/bin

.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f $(OUT)
