# Makefile for Consolidated SDL 1.2 Game
CC = gcc
TARGET = mygame
SRCS = main.c game.c puzzle.c
OBJS = $(SRCS:.c=.o)

# Windows-specific paths for vcpkg
ifeq ($(OS),Windows_NT)
    VCPKG_ROOT ?= C:/vcpkg
    CFLAGS = -Wall -Wextra -g -I$(VCPKG_ROOT)/installed/x64-windows/include -O2 -std=c99
    LIBS = -L$(VCPKG_ROOT)/installed/x64-windows/lib -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -lm
else
    CFLAGS = -Wall -Wextra -g $(shell sdl-config --cflags) -O2 -std=c99
    LIBS = $(shell sdl-config --libs) -lSDL_image -lSDL_ttf -lSDL_mixer -lm
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)
	@echo "$(TARGET) built successfully."

%.o: %.c game.h puzzle.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Cleaned project files."

run: all
	./$(TARGET)

.PHONY: all clean run
