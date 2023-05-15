SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard include/m6/*.h)
OBJECTS = $(SOURCES:.c=.o)

OUT = src/matey6

CFLAGS += -Iinclude -include m6/common.h
CFLAGS += -std=c99 -Wall -Wextra -Werror -pedantic -pedantic-errors
CFLAGS += -DM6_VERSION=\"0.0.1\"

PREFIX ?= /usr/local

.DEFAULT_GOAL := all
all: $(OUT)

$(OUT): $(OBJECTS)

$(OBJECTS): $(HEADERS)

install: $(OUT)
	install -sS -m777 $(OUT) $(PREFIX)/bin

clean:
	rm -f $(OBJECTS)
	rm -f $(OUT)
