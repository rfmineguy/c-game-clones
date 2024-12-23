OUT ?= out
CFLAGS := $(shell pkg-config --cflags raylib)
LDFLAGS := $(shell pkg-config --libs raylib)
ifeq ($(shell uname -s), Darwin)
LDFLAGS += -framework IOKit -framework Cocoa -framework OpenGL
endif

SOURCE_DIR := src
SOURCES := $(wildcard $(SOURCE_DIR)/*.c)

CC := clang
LD ?= ld

BIN := pong

.PHONY: always build clean
always:
	mkdir -p $(OUT)

build: always $(OUT)/$(BIN)

clean:
	rm -r $(OUT)

$(OUT)/$(BIN): $(SOURCES)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS) -ggdb
