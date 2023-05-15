SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)

OUT = src/matey6

CFLAGS += -Iinclude -include m6/common.h
CFLAGS += -std=c99 -Wall -Wextra -Wpedantic -Werror
CFLAGS += -DM6_VERSION=\"0.0.1\"

PREFIX ?= /usr/local

.DEFAULT_GOAL := all
all: $(OUT)

$(OUT): $(OBJECTS)

%.c: %.o

install: $(OUT)
	install -sS -m777 $(OUT) $(PREFIX)/bin

clean:
	rm -f $(OBJECTS)
	rm -f $(OUT)
